#ifndef VECTOR2_H
#define VECTOR2_H

struct Point2;
#include "Point2.h"

struct Vector2{
public:
    float x, y;
    Vector2();
    Vector2(float X, float Y);
    Vector2(Point2 p);

    Vector2 operator*(float o) const;
    Vector2 operator*(Vector2 o);
    Vector2 operator+(Vector2 o);
    Vector2 operator+(float o);
    Vector2 operator-(Vector2 o);
    Vector2 operator-();
    static Vector2 Lerp(Vector2 a, Vector2 b, float t);
};

#endif // VECTOR2_H
