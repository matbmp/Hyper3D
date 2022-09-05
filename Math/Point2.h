#ifndef POINT2_H
#define POINT2_H

struct Vector2;
#include "Vector2.h"

struct Point2{
public:
    int x,y;
    Point2();
    Point2(int x, int y);
    Point2(Vector2 v);
};

#endif // POINT2_H
