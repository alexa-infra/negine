/**
 * \file
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#include "app/demo.h"

#include <string>
#include <iostream>
#include <assert.h>
#include <sstream>

#include "renderer/md5mesh.h"

#define MD5

using base::math::deg_to_rad;

Demo::Demo(i32 width, i32 height) 
    : texture_(NULL)
    , program_(NULL)
{
    glViewport(0, 0, width, height); 

    camera_.position = Vector3(0.f, 0.f, 0.f);
    
    camera_.pitch = 0;
    camera_.head = 180 * deg_to_rad;
        
    camera_.aspect = width / (f32)height;
    camera_.fov = 50;
    camera_.zNear = 1;
    camera_.zFar = 2000;

    camera_.UpdateOrientation();

    cameraTransform_ = camera_.GetModelView(); 
    projection_ = camera_.GetProjection();
    modelTransform_.SetIdentity();
    modelTransform_.Translate(Vector3(0, 0, 150));
    modelTransform_.RotateX(-90 * deg_to_rad);
    modelTransform_.RotateZ(180 * deg_to_rad);

#ifdef MD3
    texture_ = texure_loader_.Load("european_fnt.tga");
#endif
#ifdef MD5
    texture_ = texure_loader_.Load("hellknight.png");
#endif

    program_ = LoadProgram("shader.shader");
    program_hud_ = LoadProgram("hud.shader");
    program_font_ = LoadProgram("font.shader");

#ifdef MD3
    Md3Model md3;
    md3.Load("european_fnt_v2.md3");
    md3_renderer_ = new Md3Renderer(&md3);
    md3_renderer_->Commit();
#endif

#ifdef MD5
    entity = Entity::Load("hellknight.md5mesh");
    entity->object.md5Anim = new Md5Anim;
    entity->object.md5Anim->Load("hellknight_idle2.md5anim");
    
    md5_renderer_ = new Md5Renderer(&entity->object.md5Model);
#endif

    //font test
    //string filename = "AlphaBetaBRK.ttf";
    string filename = "AmerikaSans.ttf";
    font_ = new SpriteFont(filename, 0, 100);
    title_text_ = "Just for test.";

    ParticleSystemSetting ss;
    ps_ = new ParticleSystem(ss);

    ps_renderer_ = new ParticleSystemRenderer(ps_);

    texture_ps_ = texure_loader_.Load("heart.png");

    texture_bump_ = texure_loader_.Load("hellknight_local.png");
}

Demo::~Demo() {
    delete program_;
    delete program_hud_;
    delete font_;
    delete ps_renderer_;
    delete ps_;
    delete program_font_;

#ifdef MD5
    delete entity->object.md5Anim;
    delete entity;
    delete md5_renderer_;
#endif

#ifdef MD3
    delete md3_renderer_;
#endif
}

void Demo::OnFrame(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    f32 frame_time = timer_.Elapsed() / 1000.0f;
    timer_.Reset();

    program_->Bind();
    AttributeBinding binding = program_->binding();

    modelTransform_.RotateZ(20/60.f*deg_to_rad);

    program_->set_uniform(base::opengl::UniformVars::Diffuse, texture_);
    program_->set_uniform(base::opengl::UniformVars::Bump, texture_bump_);
    program_->set_uniform(base::opengl::UniformVars::Projection, projection_);
    program_->set_uniform(base::opengl::UniformVars::Modelview, cameraTransform_ * modelTransform_);
    program_->set_uniform(base::opengl::UniformVars::CameraPos, camera_.position);
    program_->set_uniform(base::opengl::UniformVars::LightPos, Vector3(40, 110, -200));

#ifdef MD5
    static u32 counter = 0;
    counter++;
    u32 frame  = counter/2;
    f32 interp = (counter%2)/2.f;
    entity->object.md5Anim->Update(&entity->object.md5Model, frame, interp);

    md5_renderer_->Commit();
    md5_renderer_->Draw(binding);
#endif

#ifdef MD3
    md3_renderer_->Draw(binding);
#endif

    program_->Unbind();
    glDisable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    program_hud_->Bind();
    binding = program_hud_->binding();

    program_hud_->set_uniform(base::opengl::UniformVars::Diffuse, texture_ps_);
    program_hud_->set_uniform(base::opengl::UniformVars::Projection, Matrix4::GetOrtho(-150.0, 150.0, -150.0, 150.0, -500.0, 500.0));
    program_hud_->set_uniform(base::opengl::UniformVars::Modelview, Matrix4::Identity);

    ps_->update(frame_time);
    ps_renderer_->Commit();
    //ps_renderer_->Draw(binding);
    //ps_->Draw(binding);
    program_hud_->Unbind();

    program_font_->Bind();
    binding = program_font_->binding();

    program_font_->set_uniform(base::opengl::UniformVars::Diffuse, font_->texture());
    program_font_->set_uniform(base::opengl::UniformVars::Projection, Matrix4::GetOrtho(-150.0, 150.0, -150.0, 150.0, -500.0, 500.0));
    program_font_->set_uniform(base::opengl::UniformVars::Modelview, Matrix4::Identity);

    font_->SetText(Vector2(-50.f, 0.f), title_text_, Vector4(0.f, 0.f, 1.f, 1.f));
//    font_->Draw(binding);

    std::stringstream ss;
    //ss.precision(0);
    //ss.setf(std::ios::fixed,std::ios::floatfield);
    if (frame_diff_ > 0.f)
        ss << (i32)(1.f / frame_diff_) << " fps";
    std::string text = ss.str();
    font_->SetText(Vector2(-150.f, 150.f), text, Vector4(0.f, 0.f, 0.f, 1.f));
    font_->Draw(binding);
    program_font_->Unbind();

    glDisable(GL_BLEND);

    assert(glGetError() == GL_NO_ERROR);
    Application::OnFrame();

    frame_diff_ = timer_.Elapsed() / 1000.0f;
}

void Demo::OnReshape(i32 width, i32 height) {
    glViewport(0, 0, width, height);
    Application::OnReshape(width, height); 
}

void Demo::OnMotion(i32 x, i32 y, i32 dx, i32 dy) {
    camera_.head += deg_to_rad * dx;
    camera_.pitch += deg_to_rad * dy;
    camera_.UpdateOrientation();
    cameraTransform_ = camera_.GetModelView(); 

    cursor_.x += dx / (f32)width_ * 300.f;
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
    Application::OnMotion(x, y, dx, dy);
}

void Demo::OnKeyboard(u8 key, i32 x, i32 y) {
    std::string test("Key pressed: ");
    test += (char)key;
    title_text_ = test;

    if (key == 'w') {
        camera_.position += camera_.forward;
    } else if (key == 's') {
        camera_.position -= camera_.forward;
    } else if (key == 'a') {
        camera_.position -= camera_.right;
    } else if (key == 'd') {
        camera_.position += camera_.right;
    }
    //std::cout << "camera: " << camera_.position << std::endl;
    camera_.UpdateOrientation();
    cameraTransform_ = camera_.GetModelView();
}

Program* Demo::LoadProgram(const std::string& filename) {
    std::string status;
    Program* program = Program::Create(filename, status);
    if (program == NULL) {
        std::cout
            << "Error on load shader program "
            << filename << std::endl
            << status << std::endl;
        assert(false);
    }
    return program;
}
