#include "game.h"
#include "Graphics/vertex.h"
#include "Math/Matrix4.h"
#include <chrono>
#include <thread>
#include <iostream>


Game::Game()
{
}

int Game::Run(){
    std::chrono::microseconds target_interval = std::chrono::milliseconds(int((1.0F/max_framerate)*10e2));
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point last_time = std::chrono::steady_clock::now();
    Running = true;
    while(Running){
        if(!Pause){
            auto t = std::chrono::duration_cast<std::chrono::milliseconds>((std::chrono::steady_clock::now()-start));
            Update(t);
            t = std::chrono::duration_cast<std::chrono::milliseconds>((std::chrono::steady_clock::now()-start));
            Render(t);
        }
        window->update();
        app->processEvents();
        std::this_thread::sleep_until(last_time + target_interval);
        last_time = std::chrono::steady_clock::now();
        tick++;
    }
    app->exit();
    return 0;
}

void Game::UpdateWindow(){
}

HyperMaze::HyperMaze(): Game(){

}

HyperMaze::HyperMaze(MyWindow *window): Game(){
    this->window = window;
}


void HyperMaze::Init(){
    this->patternTexture = new QImage(":/pattern.jpg");
    this->patternTextureSampler = Sampler<uint>(this->patternTexture);
    this->max_framerate = 60;
    input = new InputManager();
    graphics = new CPUGraphicsDevice(700/1.0F, 700/1.0F);
    HyperbolicLighting lighting = HyperbolicLighting(0.12F, 0.02F, 50);
    HyperbolicLightSource light = HyperbolicLightSource(GyroVector(Vector3(0, 0.00F, 0)), 1.35F, Color(1,1.0F,1.0F));
    int lightmapdim = 500;
    light.lightMapFront = RenderTarget(lightmapdim, lightmapdim, RenderTargetType::DRAWDEPTH);
    light.lightMapFrontSampler = Sampler<float>(light.lightMapFront.zbuffer, light.lightMapFront.width, light.lightMapFront.height);
    light.lightMapBack = RenderTarget(lightmapdim, lightmapdim, RenderTargetType::DRAWDEPTH);
    light.lightMapBackSampler = Sampler<float>(light.lightMapBack.zbuffer, light.lightMapBack.width, light.lightMapBack.height);
    lighting.addLight(light);
    shader.lighting = lighting;

    schunk = Schunk();

    cube = O4SquareTiling::CubeMesh();

    ring1 = new GameObject(O4SquareTiling::RingMesh(0.004F, 0.03F, 10), GyroVector(Vector3(0, -0.0F, 0)));
    ring2 = new GameObject(O4SquareTiling::RingMesh(0.005F, 0.05F, 10), GyroVector(Vector3(0, -0.00F, 0)));
    gameObjects.push_back(ring1);
    gameObjects.push_back(ring2);
    cameraPos = schunk.tiles[2];
}



void HyperMaze::Update(std::chrono::milliseconds time){
    Vector2 mouse = input->mouse;
    if(mouse.x > -1 && mouse.x < 1 && mouse.y > -1 && mouse.y < 1){
        float x = mouse.x;
        float y = mouse.y;
        cameraYawPitchRoll = Vector3(-y*M_PI, x*(2.0F*M_PI),0.0F);
        cameraDir = Vector3((Matrix4::invRotationOX(-cameraYawPitchRoll.x) * Matrix4::invRotationOY(-cameraYawPitchRoll.y)) * Vector4(0,0,1));
    }
    Vector4 cameraMovement = Vector4();
    if(input->isPressed(Qt::Key_W)){
        cameraMovement = cameraMovement + cameraDir*0.1F;
    } else if(input->isPressed(Qt::Key_S)){
        cameraMovement = cameraMovement - cameraDir*0.1F;
    }
    if(input->isPressed(Qt::Key_A)){
        cameraMovement = cameraMovement + 0.1F*Vector3(-cameraDir.z, 0.0F, cameraDir.x);
    }else if(input->isPressed(Qt::Key_D)){
        cameraMovement = cameraMovement + 0.1F*Vector3(cameraDir.z, 0.0F, -cameraDir.x);
    }
    cameraMovement.y = 0;
    cameraPos = cameraPos + GyroVector(Vector3(cameraMovement)*0.25F);

    if(input->isPressed(Qt::Key_P)){
        if(tick-changeModelTick > 15){
            changeModelTick = tick;
        }
    }
    if(changeModelTick > 0){
        float a = std::min(1.0F, (tick-changeModelTick)/15.0F);
        shader.fBK = models[currentModel%2] * (1-a) + a * models[(currentModel+1)%2];
        if(a == 1.0F){
            changeModelTick = -1;
            currentModel++;
        }
    }

    if(input->isPressed(Qt::Key_O) && tick-changeShadowsTick > 3){
        shader.shadow_opt = (shader.shadow_opt + 1) % 4;
        changeShadowsTick = tick;
    }
    if(input->isPressed(Qt::Key_I) && tick-changeLightTransformTypeTick > 3){
        lightTransformType = (lightTransformType+1)%3;
        changeLightTransformTypeTick = tick;
    }


    float t = time.count() / 1000.0F;

    //shader.lighting.lights[0].lightColor = Color(std::abs(std::cos(t)), 1, std::abs(std::sin(t)));
    //shader.lighting.lights[0].originalPosition.vec = Vector3(std::cos(t*0.5F)*0.21F, 0, std::sin(t*0.5F)*0.21F);
    if(lightTransformType == 0){
        shader.lighting.lights[0].originalPosition = GyroVector();
        if(shader.shadow_opt != 2)
        shader.lighting.lights[0].originalPosition.gyr = Quaternion::CreateFromYawPitchRoll(std::sin(t/10.0F)*15.0, 0, 0);
    }

    t *= 8.0F;
    ring1->rotation = Quaternion::CreateFromYawPitchRoll(std::sin(t/20.0F)*10.0F,  std::sin(t/5.0F), 0);// *  Quaternion::CreateFromYawPitchRoll(std::sin(t/1.0F), 0, 0);
    ring2->rotation = Quaternion::CreateFromYawPitchRoll(std::cos(t/20.0F)*6.0F, -t/10.0F, 0);
}

void HyperMaze::Render(std::chrono::milliseconds time){
    shader.rotatedCamera = cameraPos;
    shader.rotatedCamera.gyr = Quaternion::CreateFromYawPitchRoll(0, cameraYawPitchRoll.x, 0) * Quaternion::CreateFromYawPitchRoll(-cameraYawPitchRoll.y, 0, 0) * cameraPos.gyr;
    if(lightTransformType == 1){
        if(shader.shadow_opt == 2){
            shader.lighting.lights[0].originalPosition = cameraPos;
        } else{
            shader.lighting.lights[0].originalPosition = shader.rotatedCamera;
        }
    }


    for(uint i = 0; i < shader.lighting.lights.size(); i ++){
        shader.TransformLight(shader.lighting.lights[i].originalPosition, shader.lighting.lights[i].position);
        shader.lighting.lights[i].minusPosition = -shader.lighting.lights[i].position;
    }

    if(shader.shadow_opt != 0){
        graphics->enableCulling = false;
        for(uint i = 0; i < shader.lighting.lights.size(); i++){
            HyperbolicLightSource light = shader.lighting.lights[i];
            shadowShader.rotatedCamera = light.originalPosition;

            shadowShader.dir = 1.0F;
            graphics->setRenderTarget(&light.lightMapFront);
            graphics->ClearZBuffer();
            for(uint j = 0; j < schunk.tiles.size(); j++){
                shadowShader.objectPos = schunk.tiles[j];
                graphics->DrawTraingles(cube[0].vertices, cube[0].indices, (Shader<PCVertex,PCPVertex>*)(&shadowShader));
                for(int k = 0; k < 4; k++){
                    if(schunk.walls[j][k]){
                        graphics->DrawTraingles(cube[2+k].vertices, cube[2+k].indices, (Shader<PCVertex,PCPVertex>*)(&shadowShader));
                    }
                }
            }
            for(uint j = 0; j < gameObjects.size(); j++){
                GameObject *go = gameObjects[j];
                shadowShader.objectPos = go->position;
                shadowShader.objectPos.gyr = go->rotation * shadowShader.objectPos.gyr;
                graphics->DrawTraingles(go->mesh.vertices, go->mesh.indices, (Shader<PCVertex,PCPVertex>*)(&shadowShader));
            }

            shadowShader.dir = -1.0F;
            graphics->setRenderTarget(&light.lightMapBack);
            graphics->ClearZBuffer();
            for(uint j = 0; j < schunk.tiles.size(); j++){
                shadowShader.objectPos = schunk.tiles[j];
                graphics->DrawTraingles(cube[0].vertices, cube[0].indices, (Shader<PCVertex,PCPVertex>*)(&shadowShader));
                for(int k = 0; k < 4; k++){
                    if(schunk.walls[j][k]){
                        graphics->DrawTraingles(cube[2+k].vertices, cube[2+k].indices, (Shader<PCVertex,PCPVertex>*)(&shadowShader));
                    }
                }
            }
            for(uint j = 0; j < gameObjects.size(); j++){
                GameObject *go = gameObjects[j];
                shadowShader.objectPos = go->position;
                shadowShader.objectPos.gyr = go->rotation * shadowShader.objectPos.gyr;
                graphics->DrawTraingles(go->mesh.vertices, go->mesh.indices, (Shader<PCVertex,PCPVertex>*)(&shadowShader));
            }

        }
    }


    graphics->enableCulling = true;
    graphics->setRenderTarget(nullptr);
    graphics->ClearZBuffer();
    graphics->Fill(uint(0x0));

    shader.enableTexture = true;
    shader.textureSampler = patternTextureSampler;
    for(uint i = 0; i < schunk.tiles.size(); i++){
        shader.objectPos = schunk.tiles[i];
        graphics->DrawTraingles(cube[0].vertices, cube[0].indices, (Shader<PCVertex,PCPVertex>*)(&shader));

        for(int j = 0; j < 4; j++){
            if(schunk.walls[i][j]){
                graphics->DrawTraingles(cube[2+j].vertices, cube[2+j].indices, (Shader<PCVertex,PCPVertex>*)(&shader));
            }
        }
    }

    shader.enableTexture = false;
    for(uint i = 0; i < gameObjects.size(); i++){
        GameObject *go = gameObjects[i];
        shader.objectPos = go->position;
        shader.objectPos.gyr = shader.objectPos.gyr * go->rotation;
        graphics->DrawTraingles(go->mesh.vertices, go->mesh.indices, (Shader<PCVertex,PCPVertex>*)(&shader));
    }

}
