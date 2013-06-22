/**
 * \file
 * \brief       MD5 model demonstration
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#include "app/app.h"
#include "render/md5mesh.h"
#include "render/md5anim.h"
#include "render/md5renderer.h"
#include "render/camera.h"
#include "render/texture.h"
#include "render/gpuprogram.h"
#include "math/matrix-inl.h"
#include "math/vec4.h"

using namespace base;
using namespace base::math;
using namespace base::opengl;
using namespace base::resource;

class Demo : public Application
{
    GpuProgram        program_;
    Camera          camera_;
    Matrix4         modelTransform_;
    Md5Renderer*    md5_renderer_;
    Entity*         entity;
    Texture*        texture_;
    Texture*        texture_bump_;
public:
    Demo()
        : program_(GL)
    {
        camera_.set_position( vec3f( 0.f, 0.f, 500.f ) );
        camera_.set_pitch( 0 );
        camera_.set_head( 180 * deg_to_rad );
        camera_.set_aspect( width_ / ( f32 )height_ );
        camera_.set_fov( 45.0f );
        camera_.set_zNear( 1 );
        camera_.set_zFar( 1000 );
        camera_.Update();
        modelTransform_ = Matrix4::Identity();
        modelTransform_ *= Matrix4::Translation( vec3f( 0, 0, 450 ) );
        modelTransform_ *= Matrix4::RotationX( -90 * deg_to_rad );
        modelTransform_ *= Matrix4::RotationZ( 180 * deg_to_rad );
        program_.create( "bump.shader.meta" );
        texture_ = GL.texture_loader()->load( "hellknight.png" );
        texture_bump_ = GL.texture_loader()->load( "hellknight_local.png" );
        entity = Entity::Load( "hellknight.md5mesh" );
        entity->object.md5Anim = new Md5Anim;
        entity->object.md5Anim->load( "hellknight_idle2.md5anim" );
        md5_renderer_ = new Md5Renderer( &entity->object.md5Model, GL );
    }
    virtual ~Demo() {
        program_.destroy();

        delete entity->object.md5Anim;
        delete entity;
        delete md5_renderer_;
    }
protected:
    void OnFrame() {
        GL.ClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
        GL.Clear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        GL.Enable( GL_DEPTH_TEST );
        program_.bind();

        ParameterMap params;
        params["diffuse"] = texture_;
        params["bump"] = texture_bump_;
        params["projection_matrix"] = camera_.GetProjection();
        params["modelview_matrix"] = camera_.GetModelView() * modelTransform_;
        params["camera_pos"] = vec4f(camera_.position(), 1.0f);
        params["light_pos"] = vec4f(camera_.position(), 1.0f);
        program_.setParams(params);

        //modelTransform_ *= Matrix4::RotationZ( 20 / 60.f * deg_to_rad );
        static u32 counter = 0;
        counter++;
        u32 frame  = counter / 10;
        f32 interp = ( counter % 10 ) / 10.f;
        entity->object.md5Anim->update( &entity->object.md5Model, frame, interp );
        md5_renderer_->commit();
        md5_renderer_->draw( );
        program_.unbind();
        GL_ASSERT(GL);
        Application::OnFrame();
    }

    void OnMotion( i32 x, i32 y, i32 dx, i32 dy ) {
        camera_.set_head( camera_.head()  + deg_to_rad * dx );

        if ( fabs ( camera_.pitch() + deg_to_rad * dy ) < base::math::pi / 4.0f ) {
            camera_.set_pitch( camera_.pitch() + deg_to_rad * dy );
        }

        camera_.Update();
    }

    void OnKeyboardDown( u8 key ) {
        const f32 speed = 5.0f;

        if ( key == 'w' ) {
            camera_.set_position( camera_.position() + camera_.forward() * speed );
        } else if ( key == 's' ) {
            camera_.set_position( camera_.position() - camera_.forward() * speed );
        } else if ( key == 'a' ) {
            camera_.set_position( camera_.position() - camera_.right()   * speed );
        } else if ( key == 'd' ) {
            camera_.set_position( camera_.position() + camera_.right()   * speed );
        }

        camera_.Update();
    }

};

int main()
{
    Demo app;
    app.Run();
    return 0;
}
