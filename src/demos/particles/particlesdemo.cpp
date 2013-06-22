/**
 * \file
 * \brief       particles demonstration
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#include "app/app.h"
#include "base/timer.h"
#include "render/particlesystem.h"
#include "render/particlesystemrenderer.h"
#include "render/texture.h"
#include "render/gpuprogram.h"
#include "math/matrix.h"
#include "math/matrix-inl.h"
#include "base/profiler.h"
#include <iostream>

using namespace base;
using namespace base::math;
using namespace base::opengl;
using namespace base::resource;

class Demo : public Application
{
    GpuProgram program_;

    ParticleSystem* ps_;
    ParticleSystemRenderer* ps_renderer_;
    vec2f cursor_;
    Timer timer_;
    ParameterMap params_;
public:
    Demo() : program_(GL) {
        ParticleSystemSetting ss;
        ss.speed = 2.0f;
        ss.texture = "heart.png";
        ps_ = new ParticleSystem( ss );
        ps_renderer_ = new ParticleSystemRenderer( ps_, GL );
        program_.create( "hud.shader.meta" );
        params_["projection_matrix"] = Matrix4::Orthographic( -150.0, 150.0, -150.0, 150.0, -500.0, 500.0 );
        params_["modelview_matrix"] = Matrix4::Identity();
        params_["diffuse"] = ps_renderer_->texture();
        Profiler::init();
    }
    virtual ~Demo() {
        program_.destroy();
        delete ps_renderer_;
        delete ps_;
        Profiler::shutdown();
    }
protected:
    void OnFrame() {

        {
            ProfilerScope("frame", Profiler::instance());
            {
                ProfilerScope("state", Profiler::instance());   
                GL.ClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
                GL.Clear( GL_COLOR_BUFFER_BIT );
                GL.Disable( GL_DEPTH_TEST );
                GL.Enable( GL_BLEND );
                GL.BlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
            }
            {
                ProfilerScope("draw", Profiler::instance());
                program_.bind();
                program_.setParams(params_);
                f32 frame_time = timer_.elapsed() / 1000.0f;
                timer_.reset();
                ps_->update( frame_time );
                ps_renderer_->commit();
                ps_renderer_->draw( &program_ );
                program_.unbind();
                GL_ASSERT(GL);
            }
            {
                ProfilerScope("swap", Profiler::instance());
                Application::OnFrame();
            }
        }
        Profiler::instance()->reportHeader(&std::cout);
        Profiler::instance()->report(&std::cout);
        std::cout.flush();
        Profiler::instance()->reset();
    }

    void OnMotion( i32 x, i32 y, i32 dx, i32 dy ) {
        cursor_.x += dx / ( f32 )width_  * 300.f;
        cursor_.y -= dy / ( f32 )height_ * 300.f;

        if ( cursor_.x > 150.f ) {
            cursor_.x = 150.f;
        }

        if ( cursor_.x < -150.f ) {
            cursor_.x = -150.f;
        }

        if ( cursor_.y > 150.f ) {
            cursor_.y = 150.f;
        }

        if ( cursor_.y < -150.f ) {
            cursor_.y = -150.f;
        }

        ps_->position = vec3f( cursor_.x, cursor_.y, 0.0f );
    }
};

int main()
{
    Demo app;
    app.Run();
    return 0;
}
