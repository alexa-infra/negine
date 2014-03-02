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

struct GameObject
{
    game::Transform* transform;
    game::Renderable* model;

    GameObject(const std::string& name, game::Scene& scene) {
        transform = scene.attach(name, new game::Transform);
        model = scene.attach(name, new game::Renderable);
    }

    ~GameObject() {
    }
};

struct GameCamera
{
    game::Transform* transform;
    game::Camera* camera;

    GameCamera(const std::string& name, game::Scene& scene) {
        transform = scene.attach(name, new game::Transform);
        camera = scene.attach(name, new game::Camera);
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
    GameObject* obj_;
    GameCamera* cam_;
    opengl::RenderPipeline pipeline_;
    u32 keypressed_;
    Timer timer_;
};
