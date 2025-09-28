#pragma once
#include "pch.h"
#include "globals.h"

struct Cell
{
    int x{}, y{};

    Cell(int x, int y) : x(x), y(y) {}
};

class Ball
{
public:
    Vec2 pos;
    Vec2 oldPos;
    float size;
    ImColor color;
    float mass;
    bool pinned;
    int id;
    Cell cell{0, 0};

    Ball(Vec2 pos, bool pinned = false, int id = 0, float size = 20.f, float mass = 0.5f, ImColor color = Colors::White) :
        pos(pos), oldPos(pos), size(size), color(color), mass(mass), pinned(pinned), id(id) {
        this->mass = 1.f - (size / 100.f);
    }

    void update(float dt);

    void checkBorders();

    void normalizeVelocity();
};