/**
 * @author  Alexey Vasilyev <alexa.infra@gmail.com>
 * @section LICENSE
 * 
 * This file is part of Negine (https://alexadotlife.com/hg/negine)
 * Licensed under LGPLv3 (or GPLv3) - see LICENSE file for details
 **/
#include "app/sdlapp.h"
#include "app/sdldemo.h"

#include <iostream>
#include <string>
#include <assert.h>

SDLDemo::SDLDemo(u32 width, u32 height)
    : SDLApp(width, height)
    , texture_(NULL)
    , program_(NULL)
    , buffer_(NULL)
{
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, width, height); 

    projection_ = base::math::Matrix4::GetOrtho(-150.0, 150.0, -150.0, 150.0, -5.0, 5.0);

    modelview_.SetIdentity();

    base::opengl::TextureInfo tex_info;
    tex_info.Filename = "european_fnt.tga";
    tex_info.MinFilter = base::opengl::TextureMinFilters::LINEAR;
    tex_info.GenerateMipmap = true;
    tex_info.Pixel = base::opengl::PixelTypes::RGBA;

    texture_ = new base::opengl::Texture;
    texture_->GenerateFromFile(tex_info);

    std::string status;
    program_ = base::opengl::Program::Create("shader.shader", status);
    if (program_ == NULL) {
        std::cout << "Error at creating shader program: " << status << std::endl;
        assert(false);
    }
    program_->Bind();

    std::vector<base::opengl::Mesh*> mesh_list 
        = base::opengl::load_md3_se("european_fnt_v2.md3");
    for (u32 i=0; i<mesh_list.size(); i++) {
        base::opengl::Mesh* m = mesh_list[i];
        base::opengl::VertexBuffer* vb = new base::opengl::VertexBuffer;
        vb->SetData(m->vertexes, m->num_vertexes, m->faces, m->num_faces);
        mesh_.push_back(vb);
        delete m;
    }

    base::opengl::ParticleSystemSetting ss;
    ps_ = new base::opengl::ParticleSystem(ss);

    timer_.Reset();
}

SDLDemo::~SDLDemo() {
    for (auto m=mesh_.begin(); m != mesh_.end(); ++m)
        delete (*m);
    delete buffer_;
    delete texture_;
    delete program_;
}

void SDLDemo::OnFrame() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

//    glDepthMask(GL_TRUE);
//    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    
    i32 pos = program_->get_attributes()["position"].Location;
    i32 tex = program_->get_attributes()["tex"].Location;
    i32 color = program_->get_attributes()["col"].Location;
    i32 normal = program_->get_attributes()["n"].Location;

    base::opengl::AttributeBinding binding;
    binding.push_back(std::make_pair(base::opengl::VertexAttrs::tagPosition, pos));
    binding.push_back(std::make_pair(base::opengl::VertexAttrs::tagTexture, tex));
    binding.push_back(std::make_pair(base::opengl::VertexAttrs::tagColor, color));
    binding.push_back(std::make_pair(base::opengl::VertexAttrs::tagNormal, normal));

    base::generic_param<base::opengl::Texture*> p(texture_);
    program_->set_uniform("diffuse", p);

    base::generic_param<base::math::Matrix4> proj(projection_);
    program_->set_uniform("projection_matrix", proj);

    base::generic_param<base::math::Matrix4> modv(modelview_);
    program_->set_uniform("modelview_matrix", modv);

//    for (auto m = mesh_.begin(); m != mesh_.end(); ++m)
//        (*m)->Draw(binding);

	ps_->Draw(binding, timer_.Elapsed());
	timer_.Reset();

    assert(glGetError() == GL_NO_ERROR);

    SDLApp::OnFrame();
}

void SDLDemo::OnMotion(f32 dx, f32 dy) {
//    modelview_.Rotate(base::math::Vector3((f32)1, (f32)0, (f32)0), -dy);
//    modelview_.Rotate(base::math::Vector3((f32)0, (f32)1, (f32)0), -dx);
    ps_->position += base::math::Vector3(dx * 10, dy * 10, 0);
}

int main(int argc, char *argv[])
{
    SDLDemo app(512, 512);
    app.Run();
 
    return 0;
}