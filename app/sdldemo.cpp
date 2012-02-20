/**
 * @author  Alexey Vasilyev <alexa.infra@gmail.com>
 * @section LICENSE
 * 
 * This file is part of Negine (https://alexadotlife.com/hg/negine)
 * Licensed under LGPLv3 (or GPLv3) - see LICENSE file for details
 **/
#include "app/sdldemo.h"

#include <iostream>
#include <string>
#include <assert.h>
#include <sstream>

using namespace base::math;
using namespace base::opengl;

SDLDemo::SDLDemo()
    : texture_(NULL)
    , program_(NULL)
    , buffer_(NULL)
{
//    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, width_, height_); 

    cameraTransform_ = Matrix4::LookAt(Vector3(0., 0., -3.), Vector3(0., 0., 0.));
    projection_ = Matrix4::GetOrtho(-150.0, 150.0, -150.0, 150.0, -5.0, 5.0);

    modelview_.SetIdentity();

    TextureInfo tex_info;
    tex_info.Filename = "heart.png";
    tex_info.MinFilter = TextureMinFilters::LINEAR;
    tex_info.GenerateMipmap = true;
    tex_info.Pixel = PixelTypes::RGBA;

    texture_ = new Texture;
    texture_->GenerateFromFile(tex_info);

    std::string status;
    program_ = Program::Create("shader.shader", status);
    if (program_ == NULL) {
        std::cout << "Error at creating shader program: " << status << std::endl;
        assert(false);
    }

    program_hud_ = Program::Create("hud.shader", status);
    if (program_hud_ == NULL) {
        std::cout << "Error at creating HUD shader program: " << status << std::endl;
        assert(false);
    }
    
    std::vector<Mesh*> mesh_list 
        = load_md3_se("european_fnt_v2.md3");
    for (u32 i=0; i<mesh_list.size(); i++) {
        Mesh* m = mesh_list[i];
        VertexBuffer* vb = new VertexBuffer;
        vb->SetData(m->vertexes, m->num_vertexes, m->faces, m->num_faces);
        mesh_.push_back(vb);
        delete m;
    }

    ParticleSystemSetting ss;
    ps_ = new ParticleSystem(ss);

    font_ = new SpriteFont("AmerikaSans.ttf", 0, 100);

    timer_.Reset();
}

SDLDemo::~SDLDemo() {
    for (auto m=mesh_.begin(); m != mesh_.end(); ++m)
        delete (*m);
    delete buffer_;
    delete texture_;
    delete program_;
    delete program_hud_;
    delete font_;
    delete ps_;
}

void SDLDemo::OnFrame() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

//    glDepthMask(GL_TRUE);
//    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    program_hud_->Bind();

    AttributeBinding binding = program_hud_->binding();

    program_hud_->set_uniform("diffuse", texture_);
    program_hud_->set_uniform("projection_matrix", projection_);
    program_hud_->set_uniform("modelview_matrix", cameraTransform_ * modelview_);

//    for (auto m = mesh_.begin(); m != mesh_.end(); ++m)
//        (*m)->Draw(binding);
    f32 frame_time = timer_.Elapsed() / 1000.0f;
    timer_.Reset();

	ps_->Draw(binding, frame_time);

    assert(glGetError() == GL_NO_ERROR);

    program_hud_->Bind();

    std::stringstream ss;
    ss.precision(2);
    ss.setf(std::ios::fixed,std::ios::floatfield);
    ss << frame_time << " sec";
    std::string text = ss.str();
    font_->SetText(-150.f, 150.f, text);

    binding = program_hud_->binding();

    program_hud_->set_uniform("diffuse", texture_);
    program_hud_->set_uniform("projection_matrix", projection_);

    base::math::Matrix4 m = cameraTransform_;
    m.Invert();
    program_hud_->set_uniform("modelview_matrix", m);

    font_->Draw(binding);

    assert(glGetError() == GL_NO_ERROR);
    Application::OnFrame();
}

void SDLDemo::OnMotion(f32 dx, f32 dy) {
//    modelview_.Rotate(Vector3((f32)1, (f32)0, (f32)0), -dy);
//    modelview_.Rotate(Vector3((f32)0, (f32)1, (f32)0), -dx);
    cursor_.x += dx / width_ * 300.f;
    cursor_.y -= dy / height_ * 300.f;
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

int main(int argc, char *argv[])
{
    SDLDemo app;
    app.Run();
 
    return 0;
}
