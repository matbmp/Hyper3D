#include "Point2.h"

Point2::Point2(): x(0), y(0) {}
Point2::Point2(int X, int Y) :x(X), y(Y) {}
Point2::Point2(Vector2 v) : x(v.x), y(v.y) {}
