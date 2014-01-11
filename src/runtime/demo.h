#pragma once

#include "app/app.h"
#include "base/timer.h"
#include "render/material.h"
#include "render/model.h"
#include "game/components/transform.h"
#include "game/components/camera.h"
#include "game/components/renderable.h"

#include <string>

using namespace base;

Application& globalApp();

class GameCamera {
public:
    game::Transform transform;
    game::Camera camera;
    game::Entity object;

    GameCamera() {
        object.add(&transform);
        object.add(&camera);
    }
};

class GameObject {
public:
    game::Renderable r;
    game::Entity object;
    GameObject() {
        object.add(&r);
    }
};

class Demo : public Application
{
public:
    Demo(const std::string& filename);
    virtual ~Demo();
protected:
    void OnFrame();
    void OnKeyboardDown(u8 key);
    void OnKeyboardUp(u8 key);
    void OnMotion(i32 x, i32 y, i32 dx, i32 dy);
    void UpdateWorld();
private:
    opengl::Renderer ren;
    GameObject obj;
    GameCamera cam;
    game::Entity root;
    u32 keypressed_;
    Timer timer_;
    opengl::Model* umesh;
    opengl::Material material;
};
