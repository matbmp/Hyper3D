#include "Math.h"

Vector3 Math::IloczynWektorowy(Vector3 v1, Vector3 v2){
    return Vector3(v1.y*v2.z - v2.y*v1.z,
                   -v1.x*v2.z + v2.x*v1.z,
                   v1.x*v2.y - v2.x*v1.y);
}
Vector3 Math::NormalnaTrojkata(Vector3 v1, Vector3 v2, Vector3 v3){
    return IloczynWektorowy(v3-v2, v3-v1);
}
Vector3 Math::SrodekTrojkata(Vector3 v1, Vector3 v2, Vector3 v3){
    return (v1+v2+v3)*(1/3.0F);
}
float Math::IloczynSkalarny(Vector3 v1, Vector3 v2){
    return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}
float Math::invSqrt(const float f){
    const float threehalfs = 1.5F;
    float y = f;

    long i = * ( long * ) &y;

    i = 0x5f3759df - ( i >> 1 );
    y = * ( float * ) &i;

    y = y * ( threehalfs - ( (f * 0.5F) * y * y ) );

    return y;
}
