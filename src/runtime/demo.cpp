#include "demo.h"
#include <cmath>
#include <boost/python.hpp>
#include "base/py.h"
#include "base/log.h"
#include "math/matrix-inl.h"
#include "engine/resourceref.h"
#include "render/renderstate.h"
#include "render/texture.h"

extern "C" NEGINE_EXPORT PyObject* PyInit_negine_runtime(void);

using namespace boost::python;

static Application* intstance_ = nullptr;

Application& globalApp() {
    ASSERT(intstance_ != nullptr);
    return *intstance_;
}

Demo::Demo(const std::string& filename) : ren(GL) {
    intstance_ = this;
    PyImport_AppendInittab("negine_core", PyInit_negine_core);
    PyImport_AppendInittab("negine_runtime", PyInit_negine_runtime);
    Py_Initialize();
    object m = import("__main__");
    object global = object(m.attr("__dict__"));

    exec_file(filename.c_str(), global, global);

    keypressed_ = 0;

    ren.init();

    cam.transform.setPosition(math::vec3f(0.f, 0.f, -5.f));
    cam.transform.setPitch(50 * math::deg_to_rad);
    cam.transform.setHead(180 * math::deg_to_rad);
    cam.camera.setPerspective(width_ / (f32)height_, 45.0f, 1, 1000);

    ResourceRef checkers("checkers");
    ResourceRef("default_fbo").setResource(nullptr);

    GL_ASSERT(GL);

    ResourceRef model("themodel");
    model.loadDefault<opengl::Model>("trunk.obj");
    umesh = model.resourceAs<opengl::Model>();

    obj.r.model_ = model;
    root.addChild(&obj.object);
    root.addChild(&cam.object);

    material.modeMap["normal"] = ResourceRef("prog1");
    ResourceRef maa("material");
    maa.setResource(&material);
    const_cast<opengl::Mesh&>(umesh->surfaceAt(0).mesh).material_ = maa;

    ren.root = &root;
    ren.camera = &cam.camera;

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
    ren.passesList.push_back(rp);

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
    ren.passesList.push_back(rp2);
}

Demo::~Demo() {
}

void Demo::OnFrame() {
    UpdateWorld();
    ren.rendering();
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
    cam.transform.turnHead(math::deg_to_rad * dx);

    if (fabs(cam.transform.pitch() + math::deg_to_rad * dy) < math::pi / 2.0f) {
        cam.transform.turnPitch(math::deg_to_rad * dy);
    }
    else if (dy != 0) {
        cam.transform.setPitch(math::pi / 2.0f * dy / fabs((f32)dy));
    }
}

void Demo::UpdateWorld() {
    const f32 speed = 0.1f;

    if (keypressed_ & 1) {
        cam.transform.moveForward(speed);
    }

    if (keypressed_ & 2) {
        cam.transform.moveBackward(speed);
    }

    if (keypressed_ & 4) {
        cam.transform.moveLeft(speed);
    }

    if (keypressed_ & 8) {
        cam.transform.moveRight(speed);
    }

    if (keypressed_ & 16) {
        if (fabs(cam.transform.pitch() + math::deg_to_rad) < math::pi / 2.0f)
            cam.transform.turnPitch(math::deg_to_rad);
    }

    if (keypressed_ & 32) {
        if (fabs(cam.transform.pitch() - math::deg_to_rad) < math::pi / 2.0f)
            cam.transform.turnPitch(-math::deg_to_rad);
    }

    if (keypressed_ & 64) {
        cam.transform.turnHead(math::deg_to_rad);
    }

    if (keypressed_ & 128) {
        cam.transform.turnHead(-math::deg_to_rad);
    }

    game::Transform::updateTree(&cam.object);
    game::Camera::updateTree(&cam.object);
}