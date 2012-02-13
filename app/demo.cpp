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

#include "GL/glew.h"
#include "GL/glut.h"

#include <utility>

using namespace base::math;

GlutSampleWindow::GlutSampleWindow(i32 width, i32 height) 
    : GlutWindow(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE, 
                 width, height)
    , texture_(NULL)
    , program_(NULL)
    , buffer_(NULL)
    , sg_(NULL)
{
    glewExperimental = GL_TRUE;

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        std::cout << glewGetErrorString(err) << std::endl;
        assert(false);
    }

    if (!GLEW_VERSION_3_3)
    {
        std::cout << "OpenGL 3.3 is not supported." << std::endl;
        assert(false);
    }

    assert(glGetError() == GL_NO_ERROR);

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, width, height); 

    cameraTransform_ = Matrix4::LookAt(Vector3(0., 0., -50.), Vector3(0., 0., 0.));
    projection_ = base::math::Matrix4::GetOrtho(-150.0, 150.0, -150.0, 150.0, -500.0, 500.0);
    modelTransform_.SetIdentity();
    //modelTransform_.Translate(base::math::Vector3(0., 0., -450.));
    //modelTransform_.Scale(base::math::Vector3(0.5, 0.5, 0.5));

    base::opengl::TextureInfo tex_info;
    tex_info.Filename = "european_fnt.tga";
    tex_info.MinFilter = base::opengl::TextureMinFilters::LINEAR;
    tex_info.GenerateMipmap = true;
    tex_info.Pixel = base::opengl::PixelTypes::RGB;

    texture_ = new base::opengl::Texture;
    texture_->GenerateFromFile(tex_info);

    std::string status;
    program_ = base::opengl::Program::Create("shader.shader", status);
    if (program_ == NULL) {
        std::cout << "Error at creating shader program: " << status << std::endl;
        assert(false);
    }
    program_->Bind();
    
    base::math::Vector4 cc(0, 0, 0, 1);

    sg_ = new base::opengl::SpriteGroup(4);
    sg_->AddSprite(base::math::Vector2(-0.5f, -0.5f), cc, 0.25f, 0.f);
    sg_->AddSprite(base::math::Vector2(-0.5f, 0.5f), cc, 0.25f, 0.f);
    sg_->AddSprite(base::math::Vector2(0.5f, 0.5f), cc, 0.25f, 0.f);
    sg_->AddSprite(base::math::Vector2(0.5f, -0.5f), cc, 0.25f, 0.f);

    std::vector<base::opengl::Mesh*> mesh_list 
        = base::opengl::load_md3_se("european_fnt_v2.md3");
    for (u32 i=0; i<mesh_list.size(); i++) {
        base::opengl::Mesh* m = mesh_list[i];
        base::opengl::VertexBuffer* vb = new base::opengl::VertexBuffer;
        vb->SetData(m->vertexes, m->num_vertexes, m->faces, m->num_faces);
        mesh_.push_back(vb);
        delete m;
    }

    //font test
    //string filename = "AlphaBetaBRK.ttf";
    string filename = "AmerikaSans.ttf";
    font = new base::opengl::SpriteFont(filename, 0, 100);
    font->SetText(-50., 0, "Just for test.");
    
    

}

GlutSampleWindow::~GlutSampleWindow() {
    for (u32 i=0; i<mesh_.size(); i++)
        delete mesh_[i];
    delete sg_;
    delete buffer_;
    delete texture_;
    delete program_;
    delete font;
}

void GlutSampleWindow::OnDisplay(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);

    f32 frame_time = GetTimer();

    //font->print(0,0,(char*)"Just for test.");

    base::opengl::AttributeBinding binding = program_->binding();

    program_->set_uniform("diffuse", texture_);
    program_->set_uniform("projection_matrix", projection_);
    program_->set_uniform("modelview_matrix", cameraTransform_ * modelTransform_);
    
    //sg_->Draw(binding);
    
    texture_->Bind();
    for (u32 i=0; i<mesh_.size(); i++) {
        mesh_[i]->Draw(binding);
    }

    base::math::Matrix4 m = cameraTransform_;
    m.Invert();
    program_->set_uniform("modelview_matrix", m);

    font->Draw(binding);
    
    assert(glGetError() == GL_NO_ERROR);
    glutSwapBuffers();
}

void GlutSampleWindow::OnReshape(i32 width, i32 height) {
    glViewport(0, 0, width, height); 
}

void GlutSampleWindow::OnMotion(i32 x, i32 y) {
//    modelview_.SetIdentity();

    static i32 old_x = 0;
    static i32 old_y = 0;

    i32 dx = x-old_x;
    i32 dy = y-old_y;

    old_x = x;
    old_y = y;
    
    modelTransform_.Rotate(base::math::Vector3((f32)dy, (f32)dx, (f32)0), 0.1f);
}

void GlutSampleWindow::OnKeyboard(u8 key, i32 x, i32 y)
{
    std::string test("Key pressed: ");
    test += (const char*)&key;
	font->SetText(-50., 0, test);
}
