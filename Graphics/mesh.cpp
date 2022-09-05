#include "mesh.h"
#include <cmath>

Mesh::Mesh()
{

}
Mesh::Mesh(std::vector<Vector3> points, std::vector<short> indices){
    this->points = points;
    this->indices = indices;
    interpolateNormals();
}
void Mesh::addPoint(Vector3 v){
    points.push_back(v);
}
void Mesh::addTriangle(short i1, short i2, short i3){
    indices.push_back(i1);
    indices.push_back(i2);
    indices.push_back(i3);
}
void Mesh::interpolateNormals(){

    int i1, i2, i3;
    interpNormals = std::vector<Vector3>(points.size(), Vector3(0,0,0));
    for(unsigned long i = 0; i < indices.size(); i+=3){
        i1 = indices.at(i);
        i2 = indices.at(i+1);
        i3 = indices.at(i+2);
        Vector3 n = Math::NormalnaTrojkata(points.at(i2),points.at(i1),points.at(i3));
        interpNormals[i1] = interpNormals[i1] + n;
        interpNormals[i2] = interpNormals[i2] + n;
        interpNormals[i3] = interpNormals[i3] + n;
    }
    for(uint i = 0; i < interpNormals.size(); i++){
        interpNormals[i].Normalize();
    }
}

Mesh Mesh::CreateSphere(){
    Mesh m;
    int n = 20;
    int c0 = 0;
    m.addPoint(Vector3(0,1,0));
    for(int i = 0; i < n-1; i++){
        float a = (i+1) * 3.14F / float(n);
        for(int j = 0; j < n; j++){
            float b = j * (2*3.14F) / float(n);

            float x = std::sin(a) * std::cos(b);
            float y = std::cos(a);
            float z = std::sin(a) * std::sin(b);
            m.addPoint(Vector3(x, y, z));
        }
    }
    int c1 = m.points.size();
    m.addPoint(Vector3(0, -1, 0));
    for (int i = 0; i < n; ++i)
      {
        auto i0 = i + 1;
        auto i1 = (i + 1) % n + 1;
        m.addTriangle(c0, i1, i0);
        i0 = i + n * (n - 2) + 1;
        i1 = (i + 1) % n + n * (n - 2) + 1;
        m.addTriangle(c1, i0, i1);
      }
    for (int j = 0; j < n - 2; j++)
      {
        auto j0 = j * n + 1;
        auto j1 = (j + 1) * n + 1;
        for (int i = 0; i < n; i++)
        {
          auto i0 = j0 + i;
          auto i1 = j0 + (i + 1) % n;
          auto i2 = j1 + (i + 1) % n;
          auto i3 = j1 + i;
          m.addTriangle(i0, i1, i2);
          m.addTriangle(i0, i2, i3);
        }
      }
    m.interpolateNormals();
    return m;
}
