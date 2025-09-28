#pragma once
#include "pch.h"
#include "globals.h"

class Ball;

class Stick
{
public:
    Ball* p0;
    Ball* p1;
    float length;
    ImColor color;

    constexpr Stick(Ball* p0, Ball* p1, float length = 100.f, ImColor color = Colors::Red) :
        p0(p0), p1(p1), length(length), color(color) {
    }

    void update();
};