#include "demo.h"
#include <cmath>
#include <boost/python.hpp>
#include "base/py.h"
#include "base/log.h"
#include "math/matrix-inl.h"
#include "engine/resourceref.h"
#include "engine/engine.h"
#include "physics/physics.h"

extern "C" NEGINE_EXPORT PyObject* PyInit_negine_runtime(void);

using namespace boost::python;

static Application* intstance_ = nullptr;

Application& globalApp() {
    ASSERT(intstance_ != nullptr);
    return *intstance_;
}

Demo::Demo(const std::string& filename) {
    intstance_ = this;

    PyImport_AppendInittab("negine_core", PyInit_negine_core);
    PyImport_AppendInittab("negine_runtime", PyInit_negine_runtime);
    Py_Initialize();
    object m = import("__main__");
    object global = object(m.attr("__dict__"));

    exec_file(filename.c_str(), global, global);

    keypressed_ = 0;

    Engine::renderer().init();

    cam_ = new GameCamera("camera1", scene_);

    cam_->transform->setPosition(math::vec3f(0.f, 0.f, -5.f));
    cam_->transform->setPitch(50 * math::deg_to_rad);
    cam_->transform->setHead(180 * math::deg_to_rad);
    cam_->camera->setPerspective(width_ / (f32)height_, 45.0f, 1, 1000);
    cam_->camera->update();

    ResourceRef checkers("checkers");
    ResourceRef("default_fbo").setResource(nullptr);

    GL_ASSERT(GL);

    ResourceRef model("themodel");
    model.loadDefault<opengl::Model>("sphere.obj");

    obj_ = new GameObject("theObject", scene_);

    obj_->transform->setPosition(math::vec3f(0, 0, 0));
    obj_->transform->setHead(0.0f);
    obj_->transform->setPitch(0.0f);
    obj_->model->model_ = model;

    opengl::Material* material = new opengl::Material;
    material->modeMap["normal"] = ResourceRef("prog1");
    ResourceRef maa("default_material", material);

    Engine::renderer().root = &scene_;
    Engine::renderer().camera = cam_->camera;

    opengl::RenderPass rp;
    rp.target = ResourceRef("fbo");
    rp.mode = "normal";
    rp.generator = "scene";
    rp.viewport = math::vec4f(0, 0, 640, 480);
    rp.clear = true;
    rp.depthTest = true;
    rp.depthWrite = true;
    rp.cullBackFace = false;
    rp.blend = false;
    rp.clearColor = math::vec4f(1.0f, 0.0f, 0.0f, 1.0f);
    Engine::renderer().passesList.push_back(rp);

    opengl::RenderPass rp2;
    rp2.target = ResourceRef("default_fbo");
    rp2.mode = "prog2";
    rp2.generator = "fullscreen";
    rp2.viewport = math::vec4f(0, 0, 640, 480);
    rp2.clear = true;
    rp2.depthTest = false;
    rp2.depthWrite = false;
    rp2.cullBackFace = false;
    rp2.blend = false;
    rp2.clearColor = math::vec4f(1.0f, 0.0f, 0.0f, 1.0f);
    Engine::renderer().passesList.push_back(rp2);

    phys::Body* plane = new phys::Body(Engine::physics(), phys::Body::Plane, 0.0f, math::vec3f(0, 1, 0));
    phys::Body* ball = new phys::Body(Engine::physics(), phys::Body::Sphere, 1.0f, math::vec3f(0, 50, 0));
    ball->setTransform(obj_->transform);
    phys::Body* ball2 = new phys::Body(Engine::physics(), phys::Body::Sphere, 1.0f, math::vec3f(0.5f, 20.f, 0.f));

    GameObject* oo = new GameObject("theObject2", scene_);

    oo->transform->setPosition(math::vec3f(0.5f, 2.0f, 0.0f));
    oo->model->model_ = model;
    oo->transform->update();
    ball2->setTransform(oo->transform);

    timer_.reset();

    cam_->transform->update();
    obj_->transform->update();
}

Demo::~Demo() {
}

void Demo::OnFrame() {
    Engine::physics().simulate(timer_.reset() / 1000.f);
    UpdateWorld();
    Engine::renderer().rendering();
    GL_ASSERT(GL);
    SDLApp::OnFrame();
}

void Demo::OnKeyboardDown(u8 key) {
    if (key == 'w') {
        keypressed_ |= 1;
    }
    else if (key == 's') {
        keypressed_ |= 2;
    }
    else if (key == 'a') {
        keypressed_ |= 4;
    }
    else if (key == 'd') {
        keypressed_ |= 8;
    }
    else if (key == 'j') {
        keypressed_ |= 16;
    }
    else if (key == 'k') {
        keypressed_ |= 32;
    }
    else if (key == 'h') {
        keypressed_ |= 64;
    }
    else if (key == 'l') {
        keypressed_ |= 128;
    }
}

void Demo::OnKeyboardUp(u8 key) {
    if (key == 'w') {
        keypressed_ ^= 1;
    }
    else if (key == 's') {
        keypressed_ ^= 2;
    }
    else if (key == 'a') {
        keypressed_ ^= 4;
    }
    else if (key == 'd') {
        keypressed_ ^= 8;
    }
    else if (key == 'j') {
        keypressed_ ^= 16;
    }
    else if (key == 'k') {
        keypressed_ ^= 32;
    }
    else if (key == 'h') {
        keypressed_ ^= 64;
    }
    else if (key == 'l') {
        keypressed_ ^= 128;
    }
}

void Demo::OnMotion(i32 x, i32 y, i32 dx, i32 dy) {
    game::Transform* cam = scene_.getTyped<game::Transform>("camera1");
    cam->turnHead(math::deg_to_rad * dx);

    if (fabs(cam->pitch() + math::deg_to_rad * dy) < math::pi / 2.0f) {
        cam->turnPitch(math::deg_to_rad * dy);
    }
    else if (dy != 0) {
        cam->setPitch(math::pi / 2.0f * dy / fabs((f32)dy));
    }
    if (dx != 0 || dy != 0)
        cam->update();
}

void Demo::UpdateWorld() {
    const f32 speed = 0.001f;

    game::Transform* cam = scene_.getTyped<game::Transform>("camera1");

    if (keypressed_ & 1) {
        cam->moveForward(speed);
    }

    if (keypressed_ & 2) {
        cam->moveBackward(speed);
    }

    if (keypressed_ & 4) {
        cam->moveLeft(speed);
    }

    if (keypressed_ & 8) {
        cam->moveRight(speed);
    }

    if (keypressed_ & 16) {
        if (fabs(cam->pitch() + math::deg_to_rad) < math::pi / 2.0f)
            cam->turnPitch(math::deg_to_rad);
    }

    if (keypressed_ & 32) {
        if (fabs(cam->pitch() - math::deg_to_rad) < math::pi / 2.0f)
            cam->turnPitch(-math::deg_to_rad);
    }

    if (keypressed_ & 64) {
        cam->turnHead(math::deg_to_rad);
    }

    if (keypressed_ & 128) {
        cam->turnHead(-math::deg_to_rad);
    }

    if (keypressed_ != 0)
        cam->update();

    //game::Transform* obj = scene_.getTyped<game::Transform>("theObject");

    //obj->turnHead(0.001f);
    //obj->update();
}
