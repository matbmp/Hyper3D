#ifndef MESH_H
#define MESH_H

#include <vector>
#include <map>
#include <cmath>
#include "vertex.h"
#include "Math.h"

struct PNCVertex;

class Mesh
{
public:
    std::vector<Vector3> points;
    std::vector<Vector3> interpNormals;
    std::vector<short> indices;
    Mesh();
    Mesh(std::vector<Vector3> points, std::vector<short> indices);
    void addPoint(Vector3 v);
    void addTriangle(short i1, short i2, short i3);
    void interpolateNormals();
    static Mesh CreateSphere();

};

template<typename T>
struct VertexMesh{
public:
    std::vector<T> vertices;
    std::vector<short> indices;


    VertexMesh(){}
    VertexMesh(std::vector<T> vertices, std::vector<short> indices):vertices(vertices), indices(indices){}

    int addVertex(T v){
        int n = vertices.size();
        vertices.push_back(v);
        return n;
    }
    void addTriangle(short i1, short i2, short i3){
        indices.push_back(i1);
        indices.push_back(i2);
        indices.push_back(i3);
    }

    // https://lindenreidblog.com/2017/12/03/simple-mesh-tessellation-triangulation-tutorial/
    // triforce triangulation
    static void Tesselate(VertexMesh<PCVertex> &m, int n){
        while(n--){
            int i1, i2, i3;
            Vector4 v1, v2, v3;

            int vc = m.vertices.size();
            std::map<int, int> edgeMap;
            for(int i = 0; i < m.indices.size(); i+=3){
                for(int j = 0; j < 3; j++){
                    i1 = m.indices[i+j];
                    i2 = m.indices[i+((j+1)%3)];
                    int eindex = std::min(i1, i2) * vc + std::max(i1, i2);
                    if(edgeMap.find(eindex) == edgeMap.end()){
                        edgeMap[eindex] = m.vertices.size();
                        int n = m.addVertex(PCVertex(Vector4::Lerp(m.vertices[i1].position, m.vertices[i2].position, 0.5F),
                                             m.vertices[i1].color,
                                             Vector2::Lerp(m.vertices[i1].texUV, m.vertices[i2].texUV, 0.5F) ));
                        m.vertices[n].normal = Vector4::Lerp(m.vertices[i1].normal, m.vertices[i2].normal, 0.5F);
                    }
                }

            }

            int e1, e2, e3;
            int s = m.indices.size();
            for(int i = 0; i < s; i+=3){
                i1 = m.indices[i];
                i2 = m.indices[i+1];
                i3 = m.indices[i+2];

                e1 = std::min(i1, i2) * vc + std::max(i1, i2);
                e2 = std::min(i2, i3) * vc + std::max(i2, i3);
                e3 = std::min(i3, i1) * vc + std::max(i3, i1);

                e1 = edgeMap[e1];
                e2 = edgeMap[e2];
                e3 = edgeMap[e3];
                m.addTriangle(i1, e1, e3);
                m.addTriangle(e1, i2, e2);
                m.addTriangle(e2, i3, e3);

                m.indices[i] = e1;
                m.indices[i+1] = e2;
                m.indices[i+2] = e3;
            }
        }
    }

};

#endif // MESH_H
