/**
 * @author  Alexey Vasilyev <alexa.infra@gmail.com>
 * @section LICENSE
 * 
 * This file is part of Negine (https://hg.alexadotlife.com/negine)
 * Licensed under LGPLv3 (or GPLv3) - see LICENSE file for details
 **/
#include "app/demo.h"

#include <string>
#include <iostream>
#include <assert.h>
#include <sstream>

Demo::Demo(i32 width, i32 height) 
    : texture_(NULL)
    , program_(NULL)
    , buffer_(NULL)
{
    glViewport(0, 0, width, height); 

    cameraTransform_ = Matrix4::LookAt(Vector3(0., 0., -50.), Vector3(0., 0., 0.));
    projection_ = Matrix4::GetOrtho(-150.0, 150.0, -150.0, 150.0, -500.0, 500.0);
    modelTransform_.SetIdentity();

    texture_ = texure_loader_.Load("european_fnt.tga");

    program_ = LoadProgram("shader.shader");
    program_hud_ = LoadProgram("hud.shader");
    program_font_ = LoadProgram("font.shader");

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
    title_text_ = "Just for test.";

    ParticleSystemSetting ss;
    ps_ = new ParticleSystem(ss);

    texture_ps_ = texure_loader_.Load("heart.png");
}

Demo::~Demo() {
    for (u32 i=0; i<mesh_.size(); i++)
        delete mesh_[i];
    delete buffer_;
    delete program_;
    delete program_hud_;
    delete font_;
    delete ps_;
    delete program_font_;
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

    glDisable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    program_hud_->Bind();
    binding = program_hud_->binding();

    program_hud_->set_uniform("diffuse", texture_ps_);
    program_hud_->set_uniform("projection_matrix", projection_);
    program_hud_->set_uniform("modelview_matrix", cameraTransform_ * modelview_);

    ps_->Draw(binding, frame_time);

    program_font_->Bind();
    binding = program_font_->binding();

    program_font_->set_uniform("diffuse", font_->texture());
    program_font_->set_uniform("projection_matrix", projection_);
    program_font_->set_uniform("modelview_matrix", cameraTransform_ * modelview_);

    font_->SetText(Vector2(-50.f, 0.f), title_text_, Vector4(0.f, 0.f, 1.f, 1.f));
    font_->Draw(binding);

    std::stringstream ss;
    //ss.precision(0);
    //ss.setf(std::ios::fixed,std::ios::floatfield);
    if (frame_diff_ > 0.f)
        ss << (i32)(1.f / frame_diff_) << " fps";
    std::string text = ss.str();
    font_->SetText(Vector2(-150.f, 150.f), text, Vector4(0.f, 0.f, 0.f, 1.f));
    font_->Draw(binding);

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
    title_text_ = test;
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