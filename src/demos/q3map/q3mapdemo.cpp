/**
 * \file
 * \brief       q3 map demonstration
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#include "app/app.h"
#include "renderer/q3map.h"
#include "renderer/camera.h"
#include "renderer/gltexture.h"
#include "renderer/glprogram.h"
#include <assert.h>

using namespace base;
using namespace base::math;
using namespace base::opengl;

class Demo : public Application {
    Program* program_;
    q3maploader* q3map_;
    Camera camera_;
    TextureLoader texure_loader_;
    Matrix4 projection_;
    Matrix4 cameraTransform_;
    u32 keypressed_;
public:
    Demo() {
        keypressed_ = 0;
        camera_.set_position(Vector3(0.f, 0.f, 500.f));
        
        camera_.set_pitch(0);
        camera_.set_head(180 * deg_to_rad);
            
        camera_.set_aspect(width_ / (f32)height_);
        camera_.set_fov(base::math::pi / 4.f);
        camera_.set_zNear(10);
        camera_.set_zFar(5000);
        camera_.Update();

        cameraTransform_ = camera_.GetModelView(); 
        projection_ = camera_.GetProjection();

        base::FileBinary fb("maps/q3dm6.bsp");
        q3map_ = new q3maploader(fb);
        q3map_->load();
        q3map_->PreloadTextures( texure_loader_ );
        
        program_ = LoadProgram("shader.shader");
    }
    virtual ~Demo() {

        delete program_;
        delete q3map_;
    }
protected:
    void OnFrame() {
        UpdateWorld();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glEnable(GL_DEPTH_TEST);

        program_->Bind();
        program_->set_uniform(base::opengl::UniformVars::Projection, projection_);
        program_->set_uniform(base::opengl::UniformVars::Modelview, cameraTransform_);

        q3map_->render(camera_, program_, texure_loader_);
        program_->Unbind();

        assert(glGetError() == GL_NO_ERROR);
        Application::OnFrame();
    }

    void OnMotion(i32 x, i32 y, i32 dx, i32 dy) {

        camera_.set_head( camera_.head()  + deg_to_rad * dx);

        if (fabs (camera_.pitch() + deg_to_rad * dy ) < base::math::pi/4.0f )
        {
            camera_.set_pitch(camera_.pitch() + deg_to_rad * dy);
        }
        camera_.Update();

        cameraTransform_ = camera_.GetModelView(); 
    }

    void OnKeyboardDown(u8 key) {

        if (key == 'w') {
            keypressed_ |= 1;
        } else if (key == 's') {
            keypressed_ |= 2;
        } else if (key == 'a') {
            keypressed_ |= 4;
        } else if (key == 'd') {
            keypressed_ |= 8;
        }
    }
    void OnKeyboardUp(u8 key) {

        if (key == 'w') {
            keypressed_ ^= 1;
        } else if (key == 's') {
            keypressed_ ^= 2;
        } else if (key == 'a') {
            keypressed_ ^= 4;
        } else if (key == 'd') {
            keypressed_ ^= 8;
        }
    }

    void UpdateWorld() {
        const f32 speed = 10.0f;
        if (keypressed_ & 0x01)
            camera_.set_position(camera_.position() + camera_.forward() * speed);
        if (keypressed_ & 0x02)
            camera_.set_position(camera_.position() - camera_.forward() * speed);
        if (keypressed_ & 0x04)
            camera_.set_position(camera_.position() - camera_.right()   * speed);
        if (keypressed_ & 0x08)
            camera_.set_position(camera_.position() + camera_.right()   * speed);
        camera_.Update();
        cameraTransform_ = camera_.GetModelView();
    }
};

int main() {
    Demo app;
    app.Run();

    return 0;
}
