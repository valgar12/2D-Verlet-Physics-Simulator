#pragma once
#include "pch.h"
#include "globals.h"

float GetDistance(Vec2 a, Vec2 b);

float GetDistanceNoSqrt(Vec2 a, Vec2 b);

Ball* GetClosestBall(Vec2 pos, int id, bool useId = true);

void CheckCollisions(std::vector<Ball*>& neighbors);

void ConnectBalls(Vec2 mousePos);

void DeleteStick(Vec2 mousePos);

void CreateFigure(Vec2 mousePos);

ImColor getRandomImColor();

template <typename T>
int findIndex(const std::vector<T>& vec, const T& value) {
    for (size_t i = 0; i < vec.size(); i++) {
        if (vec[i] == value) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

template <typename T>
bool contiene(const std::vector<T>& vec, const T& valor) {
    for (const auto& v : vec) {
        if (v == valor) {
            return true;
        }
    }
    return false;
}