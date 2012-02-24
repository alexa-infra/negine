/**
 * @author  Alexey Vasilyev <alexa.infra@gmail.com>
 * @section LICENSE
 * 
 * This file is part of Negine (https://alexadotlife.com/hg/negine)
 * Licensed under LGPLv3 (or GPLv3) - see LICENSE file for details
 **/
#include "app/demo.h"

#include <string>
#include <iostream>

#include <utility>

using namespace base;
using namespace base::math;
using namespace base::opengl;

Demo::Demo(i32 width, i32 height) 
    : texture_(NULL)
    , program_(NULL)
    , buffer_(NULL)
{
    glViewport(0, 0, width, height); 

    cameraTransform_ = Matrix4::LookAt(Vector3(0., 0., -50.), Vector3(0., 0., 0.));
    projection_ = Matrix4::GetOrtho(-150.0, 150.0, -150.0, 150.0, -500.0, 500.0);
    modelTransform_.SetIdentity();

    TextureInfo tex_info;
    tex_info.Filename = "european_fnt.tga";
    tex_info.MinFilter = TextureMinFilters::LINEAR;
    tex_info.GenerateMipmap = true;
    tex_info.Pixel = PixelTypes::RGB;

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

    //font test
    //string filename = "AlphaBetaBRK.ttf";
    string filename = "AmerikaSans.ttf";
    font_ = new SpriteFont(filename, 0, 100);
    font_->SetText(-50., 0, "Just for test.");

    ParticleSystemSetting ss;
    ps_ = new ParticleSystem(ss);

    TextureInfo tex_info_ps;
    tex_info_ps.Filename = "heart.png";
    tex_info_ps.MinFilter = TextureMinFilters::LINEAR;
    tex_info_ps.Pixel = PixelTypes::RGBA;

    texture_ps_ = new Texture;
    texture_ps_->GenerateFromFile(tex_info_ps);
}

Demo::~Demo() {
    for (u32 i=0; i<mesh_.size(); i++)
        delete mesh_[i];
    delete buffer_;
    delete texture_;
    delete program_;
    delete program_hud_;
    delete font_;
    delete ps_;
    delete texture_ps_;
}

void Demo::OnFrame(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    f32 frame_time = timer_.Elapsed() / 1000.0f;
    timer_.Reset();

    program_->Bind();
    AttributeBinding binding = program_->binding();

    program_->set_uniform("diffuse", texture_);
    program_->set_uniform("projection_matrix", projection_);
    program_->set_uniform("modelview_matrix", cameraTransform_ * modelTransform_);
    
    texture_->Bind();
    for (u32 i=0; i<mesh_.size(); i++) {
        mesh_[i]->Draw(binding);
    }

    Matrix4 m = cameraTransform_;
    m.Invert();
    program_->set_uniform("modelview_matrix", m);

    font_->Draw(binding);

    glDisable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    program_hud_->Bind();
    binding = program_hud_->binding();

    program_hud_->set_uniform("diffuse", texture_ps_);
    program_hud_->set_uniform("projection_matrix", projection_);
    program_hud_->set_uniform("modelview_matrix", cameraTransform_ * modelview_);

    ps_->Draw(binding, frame_time);
    
    glDisable(GL_BLEND);

    assert(glGetError() == GL_NO_ERROR);
    Application::OnFrame();
}

void Demo::OnReshape(i32 width, i32 height) {
    glViewport(0, 0, width, height);
    Application::OnReshape(width, height); 
}

void Demo::OnMotion(i32 x, i32 y, i32 dx, i32 dy) {
    modelTransform_.Rotate(Vector3((f32)y, (f32)x, (f32)0), 0.1f);

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
	font_->SetText(-50., 0, test);
}
