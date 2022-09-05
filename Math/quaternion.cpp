#include "quaternion.h"



Quaternion::Quaternion(){}
Quaternion::Quaternion(float x, float y, float z, float w):x(x), y(y), z(z), w(w){}

Quaternion Quaternion::CreateFromAxisAngle(Vector3 axis, float angle)
{
    float mul = angle * 0.5F;
    float s = std::sin(mul);
    return Quaternion(axis.x * s, axis.y * s, axis.z * s, std::cos(mul));
}
Quaternion Quaternion::CreateFromYawPitchRoll(float yaw, float pitch, float roll)
{
    float num = roll * 0.5f;
    float num2 = pitch * 0.5f;
    float num3 = yaw * 0.5f;
    float num4 = std::sin(num);
    float num5 = std::cos(num);
    float num6 = std::sin(num2);
    float num7 = std::cos(num2);
    float num8 = std::sin(num3);
    float num9 = std::cos(num3);
    return Quaternion(num9 * num6 * num5 + num8 * num7 * num4, num8 * num7 * num5 - num9 * num6 * num4, num9 * num7 * num4 - num8 * num6 * num5, num9 * num7 * num5 + num8 * num6 * num4);
}

 void Quaternion::Normalize()
 {
     float num = 1.0F / std::sqrt(x * x + y * y + z * z + w * w);
     x *= num;
     y *= num;
     z *= num;
     w *= num;
 }
 float Quaternion::Length()
 {
     return std::sqrt(x * x + y * y + z * z + w * w);
 }

 Quaternion Quaternion::operator*(Quaternion b) const{
      float x2 = b.x;
      float y2 = b.y;
      float z2 = b.z;
      float w2 = b.w;
      float num = y * z2 - z * y2;
      float num2 = z * x2 - x * z2;
      float num3 = x * y2 - y * x2;
      float num4 = x * x2 + y * y2 + z * z2;
      Quaternion result;
      result.x = x * w2 + x2 * w + num;
      result.y = y * w2 + y2 * w + num2;
      result.z = z * w2 + z2 * w + num3;
      result.w = w * w2 - num4;
      return result;
 }

Quaternion Quaternion::Inverse(const Quaternion &q)
 {
     float num = q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
     float num2 = 1.0F / num;
     Quaternion result;
     result.x = (0.0F - q.x) * num2;
     result.y = (0.0F - q.y) * num2;
     result.z = (0.0F - q.z) * num2;
     result.w = q.w * num2;
     return result;
 }

 Vector3 Quaternion::operator*(Vector3 value)
 {
     Vector3 result;
     float num = 2.0f * (y * value.z - z * value.y);
     float num2 = 2.0f * (z * value.x - x * value.z);
     float num3 = 2.0f * (x * value.y - y * value.x);
     result.x = value.x + num * w + (y * num3 - z * num2);
     result.y = value.y + num2 * w + (z * num - x * num3);
     result.z = value.z + num3 * w + (x * num2 - y * num);
     return result;
 }
