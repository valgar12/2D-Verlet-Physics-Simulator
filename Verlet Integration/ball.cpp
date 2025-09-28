#include "ball.h"

void Ball::update(float dt)
{
    Vec2 velocity = pos - oldPos;
    Vec2 acceleration = forces::gravity / mass;

    oldPos = pos;

    if (!pinned && velocity.x > -60.f && velocity.x < 60.f && velocity.y > -60.f && velocity.y < 60.f)
        pos = pos + velocity + acceleration * (dt * dt);
}

void Ball::checkBorders()
{
    Vec2 velocity = pos - oldPos;

    if (pos.x - size < 0)
    {
        pos.x = size;
        oldPos.x = pos.x + velocity.x * forces::restitution;
    }
    if (pos.x + size > gl::DisplaySize.x)
    {
        pos.x = gl::DisplaySize.x - size;
        oldPos.x = pos.x + velocity.x * forces::restitution;
    }
    if (pos.y - size < 0)
    {
        pos.y = size;
        oldPos.y = pos.y + velocity.y * forces::restitution;
    }
    if (pos.y + size > gl::DisplaySize.y)
    {
        pos.y = gl::DisplaySize.y - size;
        oldPos.y = pos.y + velocity.y * forces::restitution;
    }
}

void Ball::normalizeVelocity()
{
    Vec2 vel = pos - oldPos;
    float MaxVel = 60.f;

    if (vel.x > MaxVel || vel.y > MaxVel || vel.x < -MaxVel || vel.y < -MaxVel)
    {
        oldPos = pos;
    }
}