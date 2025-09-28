#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include <d3d11.h>
#include <tchar.h>
#include "pch.h"
#include "functions.h"
#include "Figures.h"
#include "gui.h"
#include "grid.h"


static ID3D11Device* g_pd3dDevice = nullptr;
static ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
static IDXGISwapChain* g_pSwapChain = nullptr;
static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
static ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int main(int, char**)
{
    ShowWindow(GetConsoleWindow(), 0);
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"ImGui Example", nullptr };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"Simulator", WS_OVERLAPPEDWINDOW, 0, 0, gl::DisplaySize.x, gl::DisplaySize.y, nullptr, nullptr, wc.hInstance, nullptr);

    Grid grid{5.f};
    ThreadPool pool(2);

    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    using Clock = std::chrono::high_resolution_clock;
    auto last_time = Clock::now();

    bool emit = true;
    
    bool done = false;
    while (!done)
    {
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
        {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
            g_ResizeWidth = g_ResizeHeight = 0;
            CreateRenderTarget();
        }

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        auto current_time = Clock::now();
        std::chrono::duration<double> elapsed = current_time - last_time;
        double deltaTime = elapsed.count();

        last_time = current_time;

        gl::drawList = ImGui::GetBackgroundDrawList();
        gl::DisplaySize = io.DisplaySize;

        ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(0, 0), io.DisplaySize, ImColor(0.f, 0.f, 0.f));

        

        if (GetAsyncKeyState(VK_INSERT) & 1)
        {
            gl::openMenu = !gl::openMenu;
        }

        if (GetAsyncKeyState(VK_SPACE) & 1)
        {
            gl::Sticks.clear();

            gl::Balls.erase(gl::Balls.begin(), gl::Balls.end());

            for (int i = 0; i < grid.numRows; i++)
            {
                for (int j = 0; j < grid.numCols; j++)
                {
                    grid.cells[i][j].clear();
                }
            }

            gl::index = 0;
        }

        if (GetAsyncKeyState(VK_F1) & 1)
        {
            for (auto ball : gl::Balls)
            {
                ball->oldPos = ball->pos;
            }
        }

        if (GetAsyncKeyState(0x57) & 1)
        {
            Vec2 mousePos = { ImGui::GetMousePos().x, ImGui::GetMousePos().y };

            auto closestBall = GetClosestBall(mousePos, 0, false);

            float distance = GetDistance(mousePos, closestBall->pos);

            if (distance <= 50.f)
            {
                Figures::ClonedBall = closestBall;
            }
        }

        if (ImGui::IsMouseClicked(2) && Figures::ClonedBall != nullptr)
        {
            Vec2 mousePos = { ImGui::GetMousePos().x, ImGui::GetMousePos().y };

            gl::Balls.push_back(new Ball(mousePos, Figures::ClonedBall->pinned,
                gl::Balls.size(), Figures::ClonedBall->size, Figures::ClonedBall->mass, Figures::ClonedBall->color));
        }

        if (ImGui::IsMouseDown(0) && !gl::openMenu)
        {
            ImVec2 mousePos = ImGui::GetMousePos();

            Ball* closestBall = GetClosestBall(Vec2{ mousePos.x, mousePos.y }, 0, false);

            if (closestBall != nullptr)
            {
                float distance = GetDistance(Vec2{ mousePos.x, mousePos.y }, closestBall->pos);

                if (distance <= 100.f)
                {
                    closestBall->pos = Vec2{ mousePos.x, mousePos.y };
                }
            }
        }

        if (Figures::turboMode)
        {
            if (ImGui::IsMouseDown(1))
            {
                Vec2 mousePos = { ImGui::GetMousePos().x, ImGui::GetMousePos().y };

                CreateFigure(mousePos);
            }
        }
        else
        {
            if (ImGui::IsMouseClicked(1))
            {
                Vec2 mousePos = { ImGui::GetMousePos().x, ImGui::GetMousePos().y };

                CreateFigure(mousePos);
            }
        }

        if (GetAsyncKeyState(0x51) & 1)
        {
            Vec2 mousePos = { ImGui::GetMousePos().x, ImGui::GetMousePos().y };

            DeleteStick(mousePos);
        }

        for (auto& ball : gl::Balls)
        {
            ball->checkBorders();
            grid.addParticle(ball);
        }

        grid.collisionDetectionWithThreadPool(pool, 4);

        for (auto& ball : gl::Balls)
        {
            ball->update(deltaTime);

            ball->checkBorders();

            if (ball->cell.x > 0 && ball->cell.x < grid.numCols - 1 && ball->cell.y > 0 && ball->cell.y < grid.numRows - 1)
            {
                auto neighbors = grid.getNeighbors(ball);
                CheckCollisions(neighbors);

            }


            gl::drawList->AddCircleFilled(ImVec2(ball->pos.x, ball->pos.y), ball->size, ball->color, 10.f);

            if (DebugInfo::ShowGridIndex)
            {
                gl::drawList->AddText(ImVec2(ball->pos.x - 10.f, ball->pos.y - 10.f), Colors::White, std::to_string(ball->cell.x).c_str());
                gl::drawList->AddText(ImVec2(ball->pos.x - 10.f, ball->pos.y), Colors::White, std::to_string(ball->cell.y).c_str());
            }
        }

        for (int i = 0; i < forces::elasticity; i++)
        {
            for (auto& stick : gl::Sticks)
            {
                stick->update();
            }
        }

        for (auto& stick : gl::Sticks)
        {
            gl::drawList->AddLine(ImVec2(stick->p0->pos.x, stick->p0->pos.y),
                ImVec2(stick->p1->pos.x, stick->p1->pos.y),
                stick->color, 2.f);
        }

        for (auto& ball : gl::Balls)
        {
            ball->checkBorders();
            grid.removeParticle(ball);
        }

        std::string FPS = "FPS: " + std::to_string(static_cast<int>(io.Framerate));
        gl::drawList->AddText(ImVec2(0, 0), Colors::White, FPS.c_str());

        std::string Particles = "Particles: " + std::to_string(gl::Balls.size());
        gl::drawList->AddText(ImVec2(0, 15), Colors::White, Particles.c_str());

        if (DebugInfo::ShowGrid)
        {
            for (int i = 0; i < grid.numCols; i++)
            {
                gl::drawList->AddLine(ImVec2(grid.cellSize * i, 0), ImVec2(grid.cellSize* i, gl::DisplaySize.y), Colors::Red);
                gl::drawList->AddLine(ImVec2(0, grid.cellSize* i), ImVec2(gl::DisplaySize.x, grid.cellSize* i), Colors::Red);
            }
        }

        DrawGui();

        ImGui::Render();
        const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        g_pSwapChain->Present(1, 0);
    }

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}

bool CreateDeviceD3D(HWND hWnd)
{
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED)
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_ResizeWidth = (UINT)LOWORD(lParam);
        g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU)
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}
