#ifndef MATH_H
#define MATH_H

#include "Math/Vector3.h"

class Math{
public:
    static Vector3 IloczynWektorowy(Vector3 v1, Vector3 v2);
    static Vector3 NormalnaTrojkata(Vector3 v1, Vector3 v2, Vector3 v3);
    static Vector3 SrodekTrojkata(Vector3 v1, Vector3 v2, Vector3 v3);
    static float IloczynSkalarny(Vector3 v1, Vector3 v2);
    static float invSqrt(const float v);
};

#endif // MATH_H




