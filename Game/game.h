#ifndef GAME_H
#define GAME_H

#include "gameobject.h"
#include "InputManager.h"
#include "Graphics/Shader.h"
#include "Graphics/cpugraphicsdevice.h"
#include "Genaration.h"
#include <chrono>

class MyWindow;
#include "mywindow.h"
#include <QObject>

class Game
{
public:
    QApplication *app;
    MyWindow *window;
    InputManager *input;
    std::vector<GameObject*> gameObjects;
    CPUGraphicsDevice *graphics;
    float max_framerate = 30;
    int tick = 0;
    bool Running;
    bool Pause;
    Game();

    int Run();
    virtual void Init(){};
    virtual void Update(std::chrono::milliseconds time){};
    virtual void Render(std::chrono::milliseconds time){};
    virtual void OnExit(){};
    void UpdateWindow();
};

class HyperMaze : public Game{
public:
    HyperbolicPCShader shader;
    HyperbolicShadowShader shadowShader;
    Vector3 cameraYawPitchRoll;
    Vector4 cameraDir;
    GyroVector cameraPos;
    Schunk schunk;
    std::vector<VertexMesh<PCVertex> > cube;
    int currentModel = 0;
    int changeModelTick;
    std::vector<float> models = {1, 0};
    int changeShadowsTick;
    int lightTransformType;
    int changeLightTransformTypeTick;
    QImage *patternTexture;
    Sampler<uint> patternTextureSampler;
    GameObject *ring1;
    GameObject *ring2;
    GameObject *ring3;

    HyperMaze();
    HyperMaze(MyWindow *window);
    void Init();
    void Update(std::chrono::milliseconds time);
    void Render(std::chrono::milliseconds time);
};

#endif // GAME_H
