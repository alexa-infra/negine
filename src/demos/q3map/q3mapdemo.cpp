/**
 * \file
 * \brief       q3 map demonstration
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#include "app/app.h"
#include "q3map.h"
#include "render/camera.h"
#include "render/texture.h"
#include "render/gpuprogram.h"
#include "render/vertexbuffer.h"
#include "math/matrix-inl.h"

using namespace base;
using namespace base::math;
using namespace base::opengl;

class Demo : public Application
{
protected:
    GpuProgram program_;
    q3maploader* q3map_;
    Camera camera_;
    Matrix4 projection_;
    Matrix4 cameraTransform_;
    u32 keypressed_;
public:
    Demo()
        : program_(GL)
    {
        buffer_ = NULL;
        q3map_ = NULL;
        keypressed_ = 0;
        //camera_.set_position(Vector3(0.f, 0.f, 500.f));
        //camera_.set_position(Vector3(221.4f, 5.0f, 63.0f));
        //camera_.set_position(Vector3(200, 100, -1200));
        //camera_.set_position(Vector3(0, 0, 0));
        camera_.set_position( vec3f( 28.4461f, 41.9335f, 300.19f ) );
        camera_.set_pitch( 0 );
        //camera_.set_head( -math::pi );
        camera_.set_head( 0 );
        camera_.set_aspect( width_ / ( f32 )height_ );
        camera_.set_fov( 45.f );
        camera_.set_zNear( 10 );
        camera_.set_zFar( 5000 );
        camera_.Update();
        cameraTransform_ = camera_.GetModelView();
        projection_ = camera_.GetProjection();
        base::FileBinary fb( "maps/q3dm6.bsp" );
        q3map_ = new q3maploader( GL, fb );
        q3map_->load();
        q3map_->PreloadTextures( *GL.texture_loader() );
        program_.create( "q3map.shader.meta" );
    }
    virtual ~Demo() {
        program_.Destroy();
        delete q3map_;
        delete buffer_;
    }
protected:
    void OnFrame() {
        UpdateWorld();
        GL.ClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
        GL.Clear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        GL.Enable( GL_DEPTH_TEST );
        program_.Bind();
        ParameterMap params;
        params["projection_matrix"] = projection_;
        params["modelview_matrix"] = cameraTransform_;
        params["clip_matrix"] = projection_ * cameraTransform_;
        //program_.setParams(params);
        q3map_->render( camera_, &program_, params, *GL.texture_loader() );
        program_.Unbind();
        GL_ASSERT(GL);
        Application::OnFrame();
    }
    VertexBuffer* buffer_;

    void OnMotion( i32 x, i32 y, i32 dx, i32 dy ) {
        /*
                camera_.set_head( camera_.head()  + deg_to_rad * dx);

                if (fabs (camera_.pitch() + deg_to_rad * dy ) < base::math::pi/2.0f )
                {
                    camera_.set_pitch(camera_.pitch() + deg_to_rad * dy );
                }
                else
                {
                    if (dy != 0 )
                        camera_.set_pitch( base::math::pi/2.0f * dy / fabs( dy ) );
                }*/
    }

    void OnKeyboardDown( u8 key ) {
        if ( key == 'w' ) {
            keypressed_ |= 1;
        } else if ( key == 's' ) {
            keypressed_ |= 2;
        } else if ( key == 'a' ) {
            keypressed_ |= 4;
        } else if ( key == 'd' ) {
            keypressed_ |= 8;
        } else if ( key == 'j' ) {
            keypressed_ |= 16;
        } else if ( key == 'k' ) {
            keypressed_ |= 32;
        } else if ( key == 'h' ) {
            keypressed_ |= 64;
        } else if ( key == 'l' ) {
            keypressed_ |= 128;
        } else if ( key == 'p' ) {
            //std::cout << "camera pos: " << camera_.position() << '\n';
            //std::cout << "camera pitch: " << camera_.pitch() << '\n';
            //std::cout << "camera head: " << camera_.head() << '\n';
            //std::cout.flush();
        } else if ( key == 'o' ) {
            for ( int i = 0; i < 6; i++ ) {
                //std::cout << camera_.planes()[i].Normal() << ' ' << camera_.planes()[i].D() << '\n';
            }

            //std::cout.flush();
        } else if ( key == 'm' ) {
            //std::cout << camera_.GetClipMatrix() << std::endl;
        }
    }
    void OnKeyboardUp( u8 key ) {
        if ( key == 'w' ) {
            keypressed_ ^= 1;
        } else if ( key == 's' ) {
            keypressed_ ^= 2;
        } else if ( key == 'a' ) {
            keypressed_ ^= 4;
        } else if ( key == 'd' ) {
            keypressed_ ^= 8;
        } else if ( key == 'j' ) {
            keypressed_ ^= 16;
        } else if ( key == 'k' ) {
            keypressed_ ^= 32;
        } else if ( key == 'h' ) {
            keypressed_ ^= 64;
        } else if ( key == 'l' ) {
            keypressed_ ^= 128;
        }
    }

    void UpdateWorld() {
        const f32 speed = 10.0f;

        if ( keypressed_ & 1 ) {
            camera_.set_position( camera_.position() + camera_.forward() * speed );
        }

        if ( keypressed_ & 2 ) {
            camera_.set_position( camera_.position() - camera_.forward() * speed );
        }

        if ( keypressed_ & 4 ) {
            camera_.set_position( camera_.position() - camera_.right()   * speed );
        }

        if ( keypressed_ & 8 ) {
            camera_.set_position( camera_.position() + camera_.right()   * speed );
        }

        if ( keypressed_ & 16 ) {
            if ( fabs( camera_.pitch() + deg_to_rad ) < math::pi / 2.0f )
                camera_.set_pitch( camera_.pitch() + deg_to_rad );
        }

        if ( keypressed_ & 32 ) {
            if ( fabs( camera_.pitch() - deg_to_rad ) < math::pi / 2.0f )
                camera_.set_pitch( camera_.pitch() - deg_to_rad );
        }

        if ( keypressed_ & 64 ) {
            camera_.set_head( camera_.head() + deg_to_rad );
        }

        if ( keypressed_ & 128 ) {
            camera_.set_head( camera_.head() - deg_to_rad );
        }

        camera_.Update();
        cameraTransform_ = camera_.GetModelView();
    }
};

int main()
{
    Demo app;
    app.Run();
    return 0;
}
