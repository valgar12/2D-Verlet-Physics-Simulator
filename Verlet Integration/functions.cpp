#include "functions.h"
#include "Figures.h"

float GetDistance(Vec2 a, Vec2 b)
{
    Vec2 delta = b - a;

    return sqrtf(delta.x * delta.x + delta.y * delta.y);
}

float GetDistanceNoSqrt(Vec2 a, Vec2 b)
{
    return std::pow(a.x - b.x, 2) + std::pow(b.y - a.y, 2);
}

Ball* GetClosestBall(Vec2 pos, int id, bool useId)
{
    float bestDistance = FLT_MAX;
    Ball* bestBall = nullptr;

    for (auto ball : gl::Balls)
    {
        if (useId && ball->id == id) continue;

        float distance = GetDistance(pos, ball->pos);

        if (distance < bestDistance)
        {
            bestDistance = distance;
            bestBall = ball;
        }
    }

    return bestBall;
}

void CheckCollisions(std::vector<Ball*>& neighbors)
{
    const float restitution = 0.8f;

    for (auto it1 = neighbors.begin(); it1 != neighbors.end(); ++it1) {
        auto& ball1 = *it1;
        for (auto it2 = std::next(it1); it2 != neighbors.end(); ++it2) {
            auto& ball2 = *it2;

            if (&ball1 == &ball2 || ball1->id == ball2->id) continue;

            Vec2 collision_axis = ball1->pos - ball2->pos;

            float dist = collision_axis.length();

            float min_dist = ball1->size + ball2->size;

            if (dist < min_dist)
            {
                Vec2 n = collision_axis / dist;

                float delta = min_dist - dist;

				if (!ball1->pinned)
				{
                    ball1->pos.x += 0.5f * delta * n.x;
                    ball1->pos.y += 0.5f * delta * n.y;
				}
                if (!ball2->pinned)
                {
                    ball2->pos.x -= 0.5f * delta * n.x;
                    ball2->pos.y -= 0.5f * delta * n.y;
                }
            }
        }
    }
}

void ConnectBalls(Vec2 mousePos)
{
    auto closestBall = GetClosestBall(mousePos, 0, false);

    if (Figures::FirstBall == nullptr)
    {
        Figures::FirstBall = closestBall;
        return;
    }

    gl::Sticks.push_back(new Stick(Figures::FirstBall, closestBall, GetDistance(Figures::FirstBall->pos,
        closestBall->pos)));
    Figures::FirstBall = nullptr;
}

void DeleteStick(Vec2 mousePos)
{
    auto closestBall = GetClosestBall(mousePos, 0, false);

    for (int i = 0; i < gl::Sticks.size(); i++)
    {
        if (i >= gl::Sticks.size()) break;

        auto stick = gl::Sticks[i];

        if (closestBall->id == stick->p0->id || closestBall->id == stick->p1->id)
        {
            gl::Sticks.erase(gl::Sticks.begin() + i);
        }
    }
}

void CreateFigure(Vec2 mousePos)
{
    if (Figures::Square)
    {
        CreateSquare(mousePos);
    }
    if (Figures::Triangle)
    {
        CreateTriangle(mousePos);
    }
    if (Figures::Circle)
    {
        CreateCircle(mousePos);
    }
    if (Figures::ball)
    {
        int index = gl::Balls.size();

        gl::Balls.push_back(new Ball(mousePos, false, index, Figures::BallSize, 1.f, getRandomImColor()));
    }
    if (Figures::StaticBall)
    {
        int index = gl::Balls.size();

        gl::Balls.push_back(new Ball(mousePos, true, index, Figures::StaticBallSize, 1.f, Colors::Pink));
    }
    if (Figures::Sticks)
    {
        ConnectBalls(mousePos);
    }
    if (Figures::Chain)
    {
        CreateChain(mousePos, Figures::ChainOffset);
    }
}

ImColor getRandomImColor() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> distrib(0.0f, 1.0f);

    float r = distrib(gen);
    float g = distrib(gen);
    float b = distrib(gen);

    return ImColor(r, g, b);
}