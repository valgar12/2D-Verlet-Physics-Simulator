#pragma once
#include "pch.h"

struct Vec2
{
    float x{ 0.f }; float y{ 0.f };

    constexpr Vec2(float x = 0, float y = 0) : x(x), y(y) {}

    constexpr Vec2 operator+(const Vec2& other) const
    {
        return Vec2{ x + other.x, y + other.y };
    }
    constexpr Vec2 operator-(const Vec2& other) const
    {
        return Vec2{ x - other.x, y - other.y };
    }
    constexpr Vec2 operator*(float value) const
    {
        return Vec2{ x * value, y * value };
    }
    constexpr Vec2 operator/(float value) const
    {
        return Vec2{ x / value, y / value };
    }
    bool operator<(const Vec2& other) const {
        return (x == other.x) ? (y < other.y) : (x < other.x);
    }
    constexpr bool operator==(const Vec2& other) const
    {
        return x == other.x && y == other.y;
    }
    constexpr void operator+=(const Vec2& other)
    {
        x = x + other.x;
        y = y + other.y;
    }
    constexpr void operator-=(const Vec2& other)
    {
        x = x - other.x;
        y = y - other.y;
    }
    float dot(const Vec2& a, const Vec2& b) {
        return a.x * b.x + a.y * b.y;
    }
    float length() {
        return std::sqrt(x * x + y * y);
    }
};

float length(const Vec2& v);
float dot(const Vec2& a, const Vec2& b);