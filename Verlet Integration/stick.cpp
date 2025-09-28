#include "stick.h"
#include "functions.h"

void Stick::update()
{
    float dx = p1->pos.x - p0->pos.x;
    float dy = p1->pos.y - p0->pos.y;
    float dist = GetDistance(p0->pos, p1->pos);
    float diff = length - dist;
    float percentage = (diff / dist) / 2;

    float offsetX = dx * percentage;
    float offsetY = dy * percentage;

    if (!p0->pinned)
    {
        p0->pos.x -= offsetX;
        p0->pos.y -= offsetY;
    }

    if (!p1->pinned)
    {
        p1->pos.x += offsetX;
        p1->pos.y += offsetY;
    }
}