/**
 * \file
 * \brief       particles demonstration
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#include "app/app.h"
#include "base/timer.h"
#include "renderer/particlesystem.h"
#include "renderer/particlesystemrenderer.h"
#include "renderer/gltexture.h"
#include "renderer/glprogram.h"
#include <assert.h>

using namespace base;
using namespace base::math;
using namespace base::opengl;
using namespace base::resource;

class Demo : public Application {
    Program* program_;
    TextureLoader texure_loader_;

    ParticleSystem* ps_;
    ParticleSystemRenderer* ps_renderer_;
    Vector2 cursor_;
    Timer timer_;
public:
    Demo() {

        ParticleSystemSetting ss;
        ss.texture = "heart.png";
        ps_ = new ParticleSystem(ss);
        ps_renderer_ = new ParticleSystemRenderer(ps_, &texure_loader_);
        
        program_ = LoadProgram("hud.shader");
    }
    virtual ~Demo() {

        delete program_;
        delete ps_renderer_;
        delete ps_;
    }
protected:
    void OnFrame() {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glDisable(GL_DEPTH_TEST);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
        program_->Bind();

        program_->set_uniform(base::opengl::UniformVars::Projection, Matrix4::Orthographic(-150.0, 150.0, -150.0, 150.0, -500.0, 500.0));
        program_->set_uniform(base::opengl::UniformVars::Modelview, Matrix4::Identity());

        f32 frame_time = timer_.Elapsed() / 1000.0f;
        timer_.Reset();
        ps_->update(frame_time);
        ps_renderer_->Commit();
        ps_renderer_->Draw(program_);
        program_->Unbind();

        assert(glGetError() == GL_NO_ERROR);
        Application::OnFrame();
    }

    void OnMotion(i32 x, i32 y, i32 dx, i32 dy) {

        cursor_.x += dx / (f32)width_  * 300.f;
        cursor_.y -= dy / (f32)height_ * 300.f;
        if (cursor_.x > 150.f)
            cursor_.x = 150.f;
        if (cursor_.x < -150.f)
            cursor_.x = -150.f;
        if (cursor_.y > 150.f)
            cursor_.y = 150.f;
        if (cursor_.y < -150.f)
            cursor_.y = -150.f;
        ps_->position = Vector3(cursor_.x, cursor_.y, 0);
    }
};

int main() {
    Demo app;
    app.Run();

    return 0;
}
