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
#include "render/wirebox.h"
#include "math/matrix-inl.h"
#include "math/vector-inl.h"

using namespace base;
using namespace base::math;
using namespace base::opengl;
using namespace base::resource;

class Demo : public Application
{
    GpuProgram        program_;
    GpuProgram        program_wirebox_;
    Camera          camera_;
    Matrix4         modelTransform_;
    WireBox*        wire_box_;
    WireBox*        camera_wirebox_;
    Md5Renderer*    md5_renderer_;
    Entity*         entity;
    Texture*        texture_;
    Texture*        texture_bump_;
public:
    Demo()
        : program_(GL)
        , program_wirebox_(GL)
    {
        camera_.set_position( Vector3( 0.f, 0.f, 500.f ) );
        camera_.set_pitch( 0 );
        camera_.set_head( 180 * deg_to_rad );
        camera_.set_aspect( width_ / ( f32 )height_ );
        camera_.set_fov( 45.0f );
        camera_.set_zNear( 1 );
        camera_.set_zFar( 1000 );
        camera_.Update();
        modelTransform_ = Matrix4::Identity();
        modelTransform_ *= Matrix4::Translation( Vector3( 0, 0, 450 ) );
        modelTransform_ *= Matrix4::RotationX( -90 * deg_to_rad );
        modelTransform_ *= Matrix4::RotationZ( 180 * deg_to_rad );
        program_.create( "bump.shader.meta" );
        texture_ = GL.texture_loader()->Load( "hellknight.png" );
        texture_bump_ = GL.texture_loader()->Load( "hellknight_local.png" );
        entity = Entity::Load( "hellknight.md5mesh" );
        entity->object.md5Anim = new Md5Anim;
        entity->object.md5Anim->Load( "hellknight_idle2.md5anim" );
        md5_renderer_ = new Md5Renderer( &entity->object.md5Model, GL );
        //program_wirebox_.CreateFromFileWithAssert( "wirebox.shader.meta" );
        wire_box_ = new WireBox( md5_renderer_->boundingBox.min,
                                 md5_renderer_->boundingBox.max );
        f32 box_radius  = 600.0f;
        camera_wirebox_ = new WireBox ( Vector3( -1 * box_radius, -1 * box_radius, -1 * box_radius ),
                                        Vector3( box_radius, box_radius, box_radius ) );
    }
    virtual ~Demo() {
        program_.Destroy();
        program_wirebox_.Destroy();

        delete entity->object.md5Anim;
        delete entity;
        delete wire_box_;
        delete camera_wirebox_;
        delete md5_renderer_;
    }
protected:
    void OnFrame() {
        GL.Clear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        GL.ClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
        GL.Enable( GL_DEPTH_TEST );
        program_.Bind();
        program_.set_uniform( "diffuse", texture_ );
        program_.set_uniform( "bump", texture_bump_ );
        //modelTransform_ *= Matrix4::RotationZ( 20 / 60.f * deg_to_rad );
        program_.set_uniform( "projection_matrix", camera_.GetProjection() );
        program_.set_uniform( "modelview_matrix", camera_.GetModelView() * modelTransform_ );
        program_.set_uniform( "camera_pos", camera_.position() );
        program_.set_uniform( "light_pos", camera_.position() );
        static u32 counter = 0;
        counter++;
        u32 frame  = counter / 10;
        f32 interp = ( counter % 10 ) / 10.f;
        entity->object.md5Anim->Update( &entity->object.md5Model, frame, interp );
        md5_renderer_->Commit();
        
        md5_renderer_->Draw( );
        program_.Unbind();
        /*program_wirebox_.Bind();
        program_wirebox_.set_uniform( "projection_matrix", camera_.GetProjection() );
        program_wirebox_.set_uniform( "modelview_matrix", camera_.GetModelView() * modelTransform_ );
        wire_box_->setMinPoint( md5_renderer_->boundingBox.min );
        wire_box_->setMaxPoint( md5_renderer_->boundingBox.max );
        wire_box_->Draw( &program_wirebox_ );
        program_wirebox_.set_uniform( "modelview_matrix", camera_.GetModelView() );
        camera_wirebox_->Draw( &program_wirebox_ );
        program_wirebox_.Unbind();*/
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
