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

using namespace base;
using namespace base::math;
using namespace base::opengl;
using namespace base::resource;

class Demo : public Application
{
    GpuProgram program_;

    ParticleSystem* ps_;
    ParticleSystemRenderer* ps_renderer_;
    Vector2 cursor_;
    Timer timer_;
public:
    Demo() : program_(GL) {
        ParticleSystemSetting ss;
        ss.speed = 2.0f;
        ss.texture = "heart.png";
        ps_ = new ParticleSystem( ss );
        ps_renderer_ = new ParticleSystemRenderer( ps_, GL );
        program_.create( "hud.shader.meta" );
    }
    virtual ~Demo() {
        program_.Destroy();
        delete ps_renderer_;
        delete ps_;
    }
protected:
    void OnFrame() {
        GL.Clear( GL_COLOR_BUFFER_BIT );
        GL.ClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
        GL.Disable( GL_DEPTH_TEST );
        GL.Enable( GL_BLEND );
        GL.BlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
        program_.Bind();
        program_.set_uniform( "projection_matrix", Matrix4::Orthographic( -150.0, 150.0, -150.0, 150.0, -500.0, 500.0 ) );
        program_.set_uniform( "modelview_matrix", Matrix4::Identity() );
        f32 frame_time = timer_.Elapsed() / 1000.0f;
        timer_.Reset();
        ps_->update( frame_time );
        ps_renderer_->Commit();
        ps_renderer_->Draw( &program_ );
        program_.Unbind();
        GL_ASSERT(GL);
        Application::OnFrame();
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

        ps_->position = Vector3( cursor_.x, cursor_.y, 0 );
    }
};

int main()
{
    Demo app;
    app.Run();
    return 0;
}
