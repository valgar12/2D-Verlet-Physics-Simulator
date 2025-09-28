#pragma once
#include "pch.h"
#include "globals.h"

void DrawGui()
{
    if (!gl::openMenu) return;

    ImGui::SetNextWindowPos(ImVec2(100.f, 100.f));
    ImGui::SetNextWindowSize(ImVec2(400.f, 600.f));
    ImGui::Begin("Forces");
    ImGui::Text("FPS: %f", ImGui::GetIO().Framerate);
    ImGui::Text("Particles: %ld", gl::Balls.size());
    ImGui::Separator();
    ImGui::Text("GRAVITY");
    ImGui::Separator();
    ImGui::SliderFloat("Gravity X", &forces::gravity.x, -500.f, 500.f);
    ImGui::SliderFloat("Gravity Y", &forces::gravity.y, -500.f, 500.f);
    ImGui::Separator();
    ImGui::Text("ELASTICITY");
    ImGui::Separator();
    ImGui::SliderInt("Elasticity", &forces::elasticity, 1, 200);
    ImGui::Separator();
    ImGui::Checkbox("Turbo Mode", &Figures::turboMode);
    ImGui::Checkbox("Show Grid", &DebugInfo::ShowGrid);
    ImGui::Checkbox("Show Ball Grid Index", &DebugInfo::ShowGridIndex);
    ImGui::Separator();
    ImGui::Separator();
    ImGui::Text("FIGURES");
    ImGui::Separator();
    ImGui::Checkbox("Square", &Figures::Square);
    ImGui::Checkbox("Triangle", &Figures::Triangle);
    ImGui::Checkbox("Circle", &Figures::Circle);
    ImGui::Checkbox("Ball", &Figures::ball);
    ImGui::Checkbox("Static Ball", &Figures::StaticBall);
    ImGui::Checkbox("Stick", &Figures::Sticks);
    ImGui::Checkbox("Chain", &Figures::Chain);
    ImGui::Separator();
    ImGui::SliderFloat("Square size", &Figures::SquareSize, 5.f, 200.f);
    ImGui::SliderFloat("Triangle size", &Figures::TriangleSize, 5.f, 200.f);
    ImGui::SliderFloat("Circle size", &Figures::CircleSize, 5.f, 200.f);
    ImGui::SliderInt("Circle segments", &Figures::CircleSegments, 5, 100);
    ImGui::SliderFloat("Ball size", &Figures::BallSize, 0.5f, 20.f);
    ImGui::SliderFloat("Static Ball size", &Figures::StaticBallSize, 5.f, 20.f);
    ImGui::SliderFloat("Chain distance", &Figures::ChainOffset, 0.f, 200.f);
    ImGui::Separator();
    ImGui::End();
}