#include "Figures.h"
#include "functions.h"

void CreateSquare(Vec2 pos)
{
    int index = gl::Balls.size();

    gl::Balls.push_back(new Ball{ Vec2{pos.x - Figures::SquareSize, pos.y - Figures::SquareSize}, false, index });
    gl::Balls.push_back(new Ball{ Vec2{pos.x + Figures::SquareSize, pos.y - Figures::SquareSize}, false, index });
    gl::Balls.push_back(new Ball{ Vec2{pos.x + Figures::SquareSize, pos.y + Figures::SquareSize}, false, index });
    gl::Balls.push_back(new Ball{ Vec2{pos.x - Figures::SquareSize, pos.y + Figures::SquareSize}, false, index });

    gl::Sticks.push_back(new Stick(gl::Balls[index], gl::Balls[index + 1], Figures::SquareSize * 2, Colors::Purple));
    gl::Sticks.push_back(new Stick(gl::Balls[index + 1], gl::Balls[index + 2], Figures::SquareSize * 2, Colors::Purple));
    gl::Sticks.push_back(new Stick(gl::Balls[index + 2], gl::Balls[index + 3], Figures::SquareSize * 2, Colors::Purple));
    gl::Sticks.push_back(new Stick(gl::Balls[index + 3], gl::Balls[index], Figures::SquareSize * 2, Colors::Purple));
    gl::Sticks.push_back(new Stick(gl::Balls[index], gl::Balls[index + 2], sqrtf(powf(Figures::SquareSize * 2, 2) + powf(Figures::SquareSize * 2, 2)), Colors::Purple));
}

void CreateTriangle(Vec2 pos)
{
    int index = gl::Balls.size();

    Vec2 p0 = { pos.x, pos.y - Figures::TriangleSize };
    Vec2 p1 = { pos.x - Figures::TriangleSize, pos.y + Figures::TriangleSize };
    Vec2 p2 = { pos.x + Figures::TriangleSize, pos.y + Figures::TriangleSize };

    gl::Balls.push_back(new Ball(Vec2{ p0 }, false, index));
    gl::Balls.push_back(new Ball(Vec2{ p1 }, false, index));
    gl::Balls.push_back(new Ball(Vec2{ p2 }, false, index));

    gl::Sticks.push_back(new Stick(gl::Balls[index], gl::Balls[index + 1], GetDistance(gl::Balls[index]->pos, gl::Balls[index + 1]->pos), Colors::Yellow));
    gl::Sticks.push_back(new Stick(gl::Balls[index + 1], gl::Balls[index + 2], GetDistance(gl::Balls[index + 1]->pos, gl::Balls[index + 2]->pos), Colors::Yellow));
    gl::Sticks.push_back(new Stick(gl::Balls[index + 2], gl::Balls[index], GetDistance(gl::Balls[index]->pos, gl::Balls[index + 2]->pos), Colors::Yellow));
}

void CreateCircle(Vec2 pos)
{
    int index = gl::Balls.size();

    float angleStep = 2 * PI / Figures::CircleSegments;

    for (int i = 0; i < Figures::CircleSegments; i++) {
        float angle = i * angleStep;

        float x = pos.x + Figures::CircleSize * std::cos(angle);
        float y = pos.y + Figures::CircleSize * std::sin(angle);

        gl::Balls.push_back(new Ball(Vec2{ x, y }, false, index));
    }
    for (int i = 0; i < Figures::CircleSegments - 1; i++)
    {
        int opposite = (i + Figures::CircleSegments / 2) % Figures::CircleSegments;

        gl::Sticks.push_back(new Stick(gl::Balls[index + i], gl::Balls[index + i + 1], GetDistance(gl::Balls[index + i]->pos, gl::Balls[index + i + 1]->pos), Colors::Lime));
        gl::Sticks.push_back(new Stick(gl::Balls[index + i], gl::Balls[index + opposite], Figures::CircleSize * 2, Colors::Lime));
    }
    gl::Sticks.push_back(new Stick(gl::Balls[index], gl::Balls[index + Figures::CircleSegments - 1], GetDistance(gl::Balls[index]->pos, gl::Balls[index + Figures::CircleSegments - 1]->pos), Colors::Lime));

}

void CreateChain(Vec2 mousePos, float offset)
{
    int index = gl::Balls.size();

    if (Figures::ChainFirstBall == nullptr)
    {
        Ball* p1 = new Ball(mousePos, true, index, Figures::StaticBallSize, 1.f, Colors::Pink);

        gl::Balls.push_back(p1);
        Figures::ChainFirstBall = p1;
        Figures::ChainFirstBallIndex = index;
        return;
    }

    Ball* p1 = Figures::ChainFirstBall;

    float distance = GetDistance(p1->pos, mousePos);

    float dx = mousePos.x - p1->pos.x;
    float dy = mousePos.y - p1->pos.y;

    float spacing = 2.f * Figures::BallSize + offset;

    int numIntermediate = static_cast<int>(distance / spacing) - 1;
    int totalBalls = numIntermediate + 2;

    Ball* p2 = new Ball(mousePos, true, Figures::ChainFirstBallIndex + totalBalls - 1, Figures::StaticBallSize, 1.f, Colors::Pink);
    gl::Balls.push_back(p2);

    float segmentLength = distance / (totalBalls - 1);

    float ux = dx / distance;
    float uy = dy / distance;

    for (int i = 1; i < totalBalls - 1; i++)
    {
        Vec2 nextPos{};
        nextPos.x = p1->pos.x + Figures::StaticBallSize + ux * segmentLength * i;
        nextPos.y = p1->pos.y + Figures::StaticBallSize + uy * segmentLength * i;

        gl::Balls.push_back(new Ball(nextPos, false, Figures::ChainFirstBallIndex + i,
            Figures::BallSize, 1.f, Colors::Orange));
        gl::Sticks.push_back(new Stick(gl::Balls[Figures::ChainFirstBallIndex + i + 1], gl::Balls[Figures::ChainFirstBallIndex + i],
            spacing));
    }
    gl::Sticks.push_back(new Stick(gl::Balls[Figures::ChainFirstBallIndex], gl::Balls[Figures::ChainFirstBallIndex + totalBalls - 1], spacing));
    Figures::ChainFirstBall = nullptr;
}