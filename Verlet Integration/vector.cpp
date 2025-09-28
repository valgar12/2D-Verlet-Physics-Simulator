#include "vector.h"

float length(const Vec2& v) {
    return std::sqrt(v.x * v.x + v.y * v.y);
}
float dot(const Vec2& a, const Vec2& b) {
    return a.x * b.x + a.y * b.y;
}