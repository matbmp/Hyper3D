#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Graphics/mesh.h"
#include "Math/Vector3.h"
#include "Math/Matrix4.h"
#include "HMath.h"

class GameObject
{
public:
    GyroVector position;
    Quaternion rotation;
    Vector4 velocity;
    VertexMesh<PCVertex> mesh;
    GameObject();
    GameObject(VertexMesh<PCVertex> mesh);
    GameObject(VertexMesh<PCVertex> mesh, GyroVector position);
    void moveH(Vector4 v);
    void transformh(Matrix4 m);
    void transformh2(Matrix4 m);
};

#endif // GAMEOBJECT_H
