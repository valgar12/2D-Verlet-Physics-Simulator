#pragma once
#include "pch.h"
#include "vector.h"
#include "colors.h"
#include "ball.h"
#include "stick.h"


class Stick;

inline const double PI = 3.14159265358979323846;

namespace gl
{
    inline ImVec2 DisplaySize{ 1920.f, 1080.f };
    inline ImDrawList* drawList{};
    inline std::vector<Ball*> Balls{};
    inline std::vector<Stick*> Sticks{};
    inline int index{ 0 };
    inline bool openMenu = false;
}

namespace forces
{
    inline Vec2 gravity{ 0.f, 200.f };
    inline float restitution = 0.1f;
    inline int elasticity = 1.f;
}

namespace Figures
{
    inline bool turboMode = false;

    inline bool Square = false;
    inline bool Triangle = false;
    inline bool Circle = false;
    inline bool ball = true;
    inline bool Sticks = false;
    inline bool StaticBall = false;
    inline bool Chain = false;

    inline float SquareSize = 50.f;
    inline float TriangleSize = 50.f;
    inline float CircleSize = 100.f;
    inline int CircleSegments = 30;
    inline float BallSize = 5.f;
    inline float StaticBallSize = 20.f;
    inline float ChainOffset = 10.f;

    inline Ball* FirstBall = nullptr;
    inline Ball* ClonedBall = nullptr;
    inline Ball* ChainFirstBall = nullptr;
    inline int ChainFirstBallIndex = 0;
}

namespace DebugInfo
{
    inline bool ShowGrid = false;
    inline bool ShowGridIndex = false;
}