#include "gameobject.h"

GameObject::GameObject()
{

}
GameObject::GameObject(VertexMesh<PCVertex> mesh):mesh(mesh){}
GameObject::GameObject(VertexMesh<PCVertex> mesh, GyroVector position):mesh(mesh), position(position){}

void GameObject::transformh(Matrix4 m){

}
