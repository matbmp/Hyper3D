#ifndef HMATH_H
#define HMATH_H

#include "Math/Vector3.h"
#include "Math/Matrix4.h"
#include "Graphics/vertex.h"
#include "Graphics/mesh.h"
#include "Math/quaternion.h"
#include <cmath>
#include <vector>
#include <cstdlib>




class HMath{
public :
    // K - zakrzywienie przestrzeni (-1 to hyperboliczna)
   static constexpr float K = -1.0F;

   static void MobiusAdditionGyration(const Vector3 &a, const Vector3 &b, Vector3 &v, Quaternion &q){
       float k = K;
       Vector3 c = k * Math::IloczynWektorowy(a, b);
       double d = 1.0 - k * Vector3::Dot(a, b);
       Vector3 t = a + b;
       v = (t * d + Math::IloczynWektorowy(c, t)) * (1.0F / (d * d + c.LengthSquared()));
       q = Quaternion(-c.x, -c.y, -c.z, d);
       q.Normalize();
   }

   static Vector3 MobiusAddition(Vector3 a, Vector3 b)
   {
       float k = -K;
       float adot = Vector3::Dot(a, a), bdot = Vector3::Dot(b, b), abdot2 = 2*Vector3::Dot(a,b);
       float x = (1 + k * (abdot2 + bdot));
       float y = (1 - k * adot);
       float z = (1 + k * (abdot2 + k * adot*bdot));
       return (x * a + y * b) * (1.0F/z);
   }

   static Quaternion Gyration(Vector3 a, Vector3 b)
   {
       Vector3 tmp = -K * Math::IloczynWektorowy(a, b);
       Quaternion result = Quaternion(tmp.x, tmp.y, tmp.z, 1.0f - K * Math::IloczynSkalarny(a, b));
       result.Normalize();
       return result;
   }
   static float PoinareMetric(Vector3 a, Vector3 b){
       return (a - b).LengthSquared() / ((-1 + a.LengthSquared()) * (-1 + b.LengthSquared()));
   }

};

class GyroVector{
public:
    Vector3 vec;
    Quaternion gyr;

    GyroVector():vec(Vector3()), gyr(Quaternion()){}
    GyroVector(Vector3 vec):vec(vec), gyr(Quaternion()){}
    GyroVector(Vector3 vec, Quaternion gyr):vec(vec), gyr(gyr){}

    GyroVector operator-()
    {
        return GyroVector(-(gyr * vec), Quaternion::Inverse(gyr));
    }

    GyroVector operator-(GyroVector b)
    {
        return *this + (-b);
    }

    GyroVector operator+(const GyroVector &gv2)
    {
        Vector3 resultVector;
        Quaternion resultGyr;
        Vector3 tmp = Quaternion::Inverse(gyr) * gv2.vec;
        HMath::MobiusAdditionGyration(vec, tmp, resultVector, resultGyr);
        return GyroVector(resultVector, gv2.gyr * gyr * resultGyr);
    }
    GyroVector operator *(float r)
    {
        float l = (float)vec.Length();
        if (r == 0 || l == 0) return GyroVector();
        float plus = std::pow((1 - HMath::K * l), r);
        float minus = std::pow((1 + HMath::K * l), r);
        float m = (-HMath::K * ((plus - minus) / (plus + minus)) / l);
        return GyroVector(vec * m);
    }
};

class HMath2{
public:
    static Vector4 translate(Vector4 origin, Vector4 vector){
        Vector4 diff =  Matrix4::translationh(vector).HMult(origin) - origin;
        return Matrix4::translationh(vector).HMult(diff);
    }
    static Matrix4 getTranslation(Vector4 origin, Vector4 vector){
        Vector4 v = Matrix4::translationh(origin).HMult(vector) - origin;
        return Matrix4::translationh(v);
    }

    static Vector4 HB_Any(Vector4 v, float d){
        return v * (1.0F/(d + std::sqrt(1+v.LengthSquared())));
    }
    static Vector4 Any_HB(Vector4 v, float d){
        float a = v.LengthSquared();
        return v * ((d + std::sqrt(d*d*a - a + 1))/(1.0F-a));
    }
    static Vector4 Any_Any(const Vector4 &v, float d1, float d2){
        return HB_Any(Any_HB(v, d1), d2);
    }

    static Vector4 HB_Poincare(Vector4 v){
        return v * (1.0F/(1+std::sqrt(1+v.LengthSquared())));
    }
    static Vector4 HB_BK(Vector4 v){
        return v * (1.0F/std::sqrt(1+v.LengthSquared()));
    }
    static Vector4 Poincare_HB(Vector4 v){
        return v * (2.0F/(1-v.LengthSquared()));
    }
    static Vector4 BK_HB(Vector4 v){
        return v * (1.0F/std::sqrt(1-v.LengthSquared()));
    }

    static Vector3 BK_Poincare(Vector3 p)
    {
        return p * (1.0F/(std::sqrt(std::fmax(0.0f, 1.0f - p.LengthSquared())) + 1.0f));
    }
    static Vector3 Poincare_BK(Vector3 p)
    {
        return p * 2.0f * (1.0F/(1.0f + p.LengthSquared()));
    }
    static Vector4 BeltriamiNormalToPoincare(Vector4 v, Vector4 n){
        Vector3 v2 = (GyroVector(v) + GyroVector(n*0.6F)).vec;
        Vector4 result = BK_Poincare(v2) - BK_Poincare(v);
        result.Normalize();
        return result;
    }

    static float sinh(float x){
        return (std::exp(x) - std::exp(-x))/2.0F;
    }
    static float cosh(float x){
        return (std::exp(x) + std::exp(-x))/2.0F;
    }
    static float arcosh(float x){
        return std::log(x+std::sqrt(x*x-1));
    }

    static Matrix4 reflection(Vector4 v){
        Matrix4 mink = Matrix4::identity();
        mink.x44 = -1;
        v.w = v.Hw();
        float mult = (2.0F/MinkowskiProduct(v, v));
        Matrix4 result = Matrix4::identity() - (v.multiplyToMatrix(v)*mink) * mult;
        return result;
    }
    static Vector4 Midpoint(Vector4 a, Vector4 b){
        Vector4 result = (a * std::sqrt(MinkowskiProduct(b,b)*MinkowskiProduct(a,b))) + (b * std::sqrt(MinkowskiProduct(a,a)*MinkowskiProduct(a,b)));
        return result;
    }
    static Matrix4 Translation(Vector4 a, Vector4 b){
        return reflection(Midpoint(a, b)) * reflection(a);
    }
    static float Distance(Vector4 v1, Vector4 v2){
        float w1 = v1.Hw();
        float w2 = v2.Hw();
        return std::sqrt(-(w1-w2)*(w1-w2) + (v1.x-v2.x)*(v1.x-v2.x) + (v1.y-v2.y)*(v1.y-v2.y) + (v1.z-v2.z)*(v1.z-v2.z));
    }
    static float MinkowskiProduct(Vector4 v1, Vector4 v2){
        float w1 = v1.Hw();
        float w2 = v2.Hw();
        float result = v1.x*v2.x + v1.y*v2.y + v1.z*v2.z - w1*w2;
        return result;
    }
    static Vector4 order4SquareTilingVector(){
        int n = 5;
        float rr = std::sqrt(std::cos(M_PI / n + M_PI / 4) / std::cos(M_PI / n - M_PI / 4)); //half of tile diagonal
        Vector4 v1 = Matrix4::rotationOY(M_PI/4.0F)*Vector4(rr, 0, 0);
        Vector4 v2= Matrix4::rotationOY(-M_PI/4.0F)*Vector4(rr, 0, 0);
        v1 = (Poincare_HB(v1));
        v2 = (Poincare_HB(v2));
        float HBTileWidth = Distance(v1, v2);
        return Vector4(0, 0, HBTileWidth*0.7945F);
        //return Vector4(0, 0, HBTileWidth*0.95F);
    }
};

class O4SquareTiling{
private:
    static VertexMesh<PCVertex> Tile(Color color){
        int n = 5;
        float rr = std::sqrt(std::cos(M_PI / n + M_PI / 4) / std::cos(M_PI / n - M_PI / 4)); //half of tile diagonal
        Vector4 r = Matrix4::rotationOY(M_PI/4.0F)*Vector4(rr, 0, 0);
        VertexMesh<PCVertex> mesh = VertexMesh<PCVertex>(
                    std::vector<PCVertex>{
                        PCVertex(r, color, Vector2(1, 0)),
                        PCVertex(Matrix4::rotationOY(M_PI*0.5F)*r, color, Vector2(0,0)),
                        PCVertex(Matrix4::rotationOY(M_PI)*r, color, Vector2(0,1)),
                        PCVertex(Matrix4::rotationOY(M_PI*1.5F)*r,color, Vector2(1,1))
                    },
                    std::vector<short>{
                        0,1,2,
                        0,2,3
                    });
        return mesh;
    }
public:
    static void TesselatePoincare_BK(VertexMesh<PCVertex> &m, int n){
        for(uint i = 0; i < m.vertices.size(); i++){
            m.vertices[i].position = HMath2::Poincare_BK(m.vertices[i].position);
        }
        m.Tesselate(m, n);
        for(uint i = 0; i < m.vertices.size(); i++){
            m.vertices[i].position = HMath2::BK_Poincare(m.vertices[i].position);
        }
    }
    static float TileWidth(){
        int n = 5;
        float rr = std::sqrt(std::cos(M_PI / n + M_PI / 4) / std::cos(M_PI / n - M_PI / 4)); //half of tile diagonal
        Vector4 v1 = Matrix4::rotationOY(M_PI/4.0F)*Vector4(rr, 0, 0);
        Vector4 v2= Matrix4::rotationOY(7*M_PI/4.0F)*Vector4(rr, 0, 0);
        GyroVector mid = GyroVector(Vector3(v1)) + ((-GyroVector(Vector3(v1)) + GyroVector(Vector3(v2))) * 0.5F);
        float rrr = (mid * 2).vec.Length();
        return 0.4855F;
        return HMath2::HB_Poincare(Vector4(1.062F, 0, 0)).x;
        return rrr;
    }
    static VertexMesh<PCVertex> RingMesh(float s, float r, float n){ // cubesize, radius, cubenum
        Color c = Color::FromHSV(0, 1, 1);
        VertexMesh<PCVertex> ring = VertexMesh<PCVertex>(
                    std::vector<PCVertex>{
                        PCVertex(Vector3(-s, -s, -s), c),
                        PCVertex(Vector3(-s, s, -s), c),
                        PCVertex(Vector3(-s, s, s), c),
                        PCVertex(Vector3(-s, -s, s), c),
                        PCVertex(Vector3(s, -s, -s), c),
                        PCVertex(Vector3(s, s, -s), c),
                        PCVertex(Vector3(s, s, s), c),
                        PCVertex(Vector3(s, -s, s), c)
                    },
                    std::vector<short>{
                        0, 2, 1,
                        0, 3, 2,
                        0, 5, 4,
                        0, 1, 5,
                        1, 6, 5,
                        1, 2, 6,
                        2, 7, 6,
                        2, 3, 7,
                        3, 4, 7,
                        3, 0, 4,
                        4, 6, 7,
                        4, 5, 6
                    });
        for(uint i = 0; i < ring.vertices.size(); i++){
            ring.vertices[i].normal = ring.vertices[i].position;
            ring.vertices[i].normal.Normalize();
        }

        uint vs = ring.vertices.size();
        uint is = ring.indices.size();
        for(int i = 1; i < n; i++){
            float a = ((float)i)/n;
            a *= 2 * M_PI;
            Vector3 rr = Vector3(std::cos(a), 0, std::sin(a)) * r;
            GyroVector rot = GyroVector(Vector3(), Quaternion::CreateFromYawPitchRoll(a, 0, 0));
            Color c = Color::FromHSV(a / (2*M_PI), 1, 1);
            for(uint j = 0; j < vs; j++){
                ring.vertices.push_back(ring.vertices[j]);
                ring.vertices[i*vs + j].position = Vector4((GyroVector(ring.vertices[i*vs + j].position) + GyroVector(rr)).vec);
                ring.vertices[i*vs + j].color = c;
                //ring.vertices[i*vs + j].normal = (rot + GyroVector(ring.vertices[i*vs + j].normal)).vec;
            }
            for(uint j = 0; j < is; j+=3){
                ring.addTriangle(i*vs + ring.indices[j], i*vs + ring.indices[j+1], i*vs + ring.indices[j + 2]);
            }

        }

        for(uint i = 0; i < vs; i++){
            ring.vertices[i].position = Vector4((GyroVector(ring.vertices[i].position) + GyroVector(Vector3(r, 0, 0))).vec);
        }
        TesselatePoincare_BK(ring, 1);
        return ring;
    }
    static std::vector<VertexMesh<PCVertex>> CubeMesh(){
        std::vector<VertexMesh<PCVertex> > cube;
        VertexMesh<PCVertex> tile = Tile(Color(1,1,1));
        VertexMesh<PCVertex> mesh;

        GyroVector d = GyroVector(Vector3(0, -TileWidth(), 0));
        GyroVector hd = d*0.5F;


        mesh = tile;
        for(uint i = 0; i < mesh.vertices.size(); i++){
            mesh.vertices[i].position = Vector4((GyroVector(Vector3(mesh.vertices[i].position)) + hd).vec);
            mesh.vertices[i].position = HMath2::HB_BK(HMath2::Poincare_HB(mesh.vertices[i].position));
            mesh.vertices[i].normal = Vector4(0, 1, 0);
        }
        cube.push_back(mesh);

        Vector4 g0 = mesh.vertices[0].position;
        Vector4 g1 = mesh.vertices[1].position;
        Vector4 g2 = mesh.vertices[2].position;
        Vector4 g3 = mesh.vertices[3].position;

        mesh = tile;
        for(uint i = 0; i < mesh.vertices.size(); i++){
            mesh.vertices[i].position = Vector4((GyroVector(Vector3(mesh.vertices[i].position)) - hd).vec);
            mesh.vertices[i].position = HMath2::HB_BK(HMath2::Poincare_HB(mesh.vertices[i].position));
            mesh.vertices[i].normal = Vector4(0, -1, 0);
        }
        cube.push_back(mesh);

        Vector4 g4 = mesh.vertices[0].position;
        Vector4 g5 = mesh.vertices[1].position;
        Vector4 g6 = mesh.vertices[2].position;
        Vector4 g7 = mesh.vertices[3].position;


        mesh = tile;
        mesh.vertices[0].position = g4;
        mesh.vertices[1].position = g5;
        mesh.vertices[2].position = g1;
        mesh.vertices[3].position = g0;
        for(uint i = 0; i < mesh.vertices.size(); i++){
            mesh.vertices[i].normal = Vector4(0, 0, 1);
        }
        cube.push_back(mesh);

        mesh = tile;
        mesh.vertices[0].position = g5;
        mesh.vertices[1].position = g6;
        mesh.vertices[2].position = g2;
        mesh.vertices[3].position = g1;
        for(uint i = 0; i < mesh.vertices.size(); i++){
            mesh.vertices[i].normal = Vector4(1, 0, 0);
        }
        cube.push_back(mesh);

        mesh = tile;
        mesh.vertices[0].position = g6;
        mesh.vertices[1].position = g7;
        mesh.vertices[2].position = g3;
        mesh.vertices[3].position = g2;
        for(uint i = 0; i < mesh.vertices.size(); i++){
            mesh.vertices[i].normal = Vector4(0, 0, -1);
        }
        cube.push_back(mesh);

        mesh = tile;
        mesh.vertices[0].position = g7;
        mesh.vertices[1].position = g4;
        mesh.vertices[2].position = g0;
        mesh.vertices[3].position = g3;
        for(uint i = 0; i < mesh.vertices.size(); i++){
            mesh.vertices[i].normal = Vector4(-1, 0, 0);
        }
        cube.push_back(mesh);

        for(uint i = 0; i < cube.size(); i++){

            for(uint j = 0; j < cube[i].vertices.size(); j++){
                cube[i].vertices[j].normal = HMath2::BeltriamiNormalToPoincare(cube[i].vertices[j].position, cube[i].vertices[j].normal);
            }

            cube[i].Tesselate(cube[i], 3);
            for(uint j = 0; j < cube[i].vertices.size(); j++){
                cube[i].vertices[j].position = Vector4(HMath2::BK_Poincare(Vector3(cube[i].vertices[j].position)));
            }
        }

        return cube;
    }
};

#endif // HMATH_H
