/**
 * \file
 * \brief       MD5 model demonstration
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#include "app/app.h"
#include "renderer/md5mesh.h"
#include "renderer/md5anim.h"
#include "renderer/md5renderer.h"
#include "renderer/camera.h"
#include "renderer/gltexture.h"
#include "renderer/glprogram.h"
#include "renderer/wirebox.h"
#include <assert.h>

using namespace base;
using namespace base::math;
using namespace base::opengl;
using namespace base::resource;

class Demo : public Application {
    Program*        program_;
    Program*        program_wirebox_;
    Camera          camera_;
    TextureLoader   texure_loader_;
    Matrix4         projection_;
    Matrix4         modelview_;
    Matrix4         cameraTransform_;
    Matrix4         modelTransform_;
    WireBox*        wire_box_;
    WireBox*        camera_wirebox_;
    Md5Renderer*    md5_renderer_;
    Entity*         entity;
    Texture*        texture_;
    Texture*        texture_bump_;
public:
    Demo() {
        camera_.set_position(Vector3(0.f, 0.f, 500.f));
        
        camera_.set_pitch(0);
        camera_.set_head(180 * deg_to_rad);
            
        camera_.set_aspect(width_ / (f32)height_);
        camera_.set_fov(base::math::pi / 4.f);
        camera_.set_zNear(10);
        camera_.set_zFar(5000);

        cameraTransform_ = camera_.GetModelView(); 
        projection_ = camera_.GetProjection();
        modelTransform_.SetIdentity();
        modelTransform_.Translate(Vector3(0, 0, 450));
        modelTransform_.RotateX(-90 * deg_to_rad);
        modelTransform_.RotateZ(180 * deg_to_rad);

        program_ = LoadProgram("shader.shader");

        texture_ = texure_loader_.Load("hellknight.png");
        texture_bump_ = texure_loader_.Load("hellknight_local.png");

        entity = Entity::Load("hellknight.md5mesh");
        entity->object.md5Anim = new Md5Anim;
        entity->object.md5Anim->Load("hellknight_idle2.md5anim");
        
        md5_renderer_ = new Md5Renderer(&entity->object.md5Model);

        program_wirebox_ = LoadProgram("wirebox.shader");
        wire_box_ = new WireBox(md5_renderer_->boundingBox.min, 
                              md5_renderer_->boundingBox.max);

        f32 box_radius  = 600.0f;
        camera_wirebox_ = new WireBox (Vector3(-1 * box_radius, -1 * box_radius, -1 * box_radius),
                                       Vector3(box_radius, box_radius, box_radius));

    }
    virtual ~Demo() {

        delete program_;
        delete entity->object.md5Anim;
        delete entity;
        delete wire_box_;
        delete camera_wirebox_;
        delete md5_renderer_;
    }
protected:
    void OnFrame() {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glEnable(GL_DEPTH_TEST);

        program_->Bind();
        program_->set_uniform(base::opengl::UniformVars::Diffuse, texture_);
        program_->set_uniform(base::opengl::UniformVars::Bump, texture_bump_);

        modelTransform_.RotateZ(20/60.f*deg_to_rad);

        program_->set_uniform(base::opengl::UniformVars::Projection, projection_);
        program_->set_uniform(base::opengl::UniformVars::Modelview, cameraTransform_ * modelTransform_);
        program_->set_uniform(base::opengl::UniformVars::CameraPos, camera_.position());
        program_->set_uniform(base::opengl::UniformVars::LightPos, Vector3(40, 110, -200));


        static u32 counter = 0;
        counter++;
        u32 frame  = counter/2;
        f32 interp = (counter%2)/2.f;
        entity->object.md5Anim->Update(&entity->object.md5Model, frame, interp);

        md5_renderer_->Commit();
        AttributeBinding binding = program_->binding();
        md5_renderer_->Draw(binding);

        program_->Unbind();

        program_wirebox_->Bind();
        program_wirebox_->set_uniform(base::opengl::UniformVars::Projection, projection_);
        program_wirebox_->set_uniform(base::opengl::UniformVars::Modelview, cameraTransform_ * modelTransform_);    

        wire_box_->setMinPoint(md5_renderer_->boundingBox.min);
        wire_box_->setMaxPoint(md5_renderer_->boundingBox.max);
        wire_box_->Draw(program_wirebox_);

        program_wirebox_->set_uniform(base::opengl::UniformVars::Modelview, cameraTransform_);
        camera_wirebox_->Draw(program_wirebox_);

        program_wirebox_->Unbind();

        assert(glGetError() == GL_NO_ERROR);
        Application::OnFrame();
    }

    void OnMotion(i32 x, i32 y, i32 dx, i32 dy) {

        camera_.set_head( camera_.head()  + deg_to_rad * dx);

        if (fabs (camera_.pitch() + deg_to_rad * dy ) < base::math::pi/4.0f )
        {
            camera_.set_pitch(camera_.pitch() + deg_to_rad * dy);
        }

        cameraTransform_ = camera_.GetModelView(); 
    }

    void OnKeyboard(u8 key, i32 x, i32 y) {

        const f32 speed = 5.0f;

        if (key == 'w') {
            camera_.set_position(camera_.position() + camera_.forward() * speed);
        } else if (key == 's') {
            camera_.set_position(camera_.position() - camera_.forward() * speed);
        } else if (key == 'a') {
            camera_.set_position(camera_.position() - camera_.right()   * speed);
        } else if (key == 'd') {
            camera_.set_position(camera_.position() + camera_.right()   * speed);
        }
        cameraTransform_ = camera_.GetModelView();
    }

};

int main() {
    Demo app;
    app.Run();

    return 0;
}
