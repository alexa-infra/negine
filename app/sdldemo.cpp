#include "app/sdlapp.h"
#include "app/sdldemo.h"

#include <iostream>
#include <string>

SDLDemo::SDLDemo(u32 width, u32 height)
    : SDLApp(width, height)
    , texture_(NULL)
    , program_(NULL)
    , buffer_(NULL)
{
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, width, height); 

    projection_ = base::math::Matrix4::GetOrtho(-150.0, 150.0, -150.0, 150.0, -500.0, 500.0);

    modelview_.SetIdentity();

    base::opengl::TextureInfo tex_info;
    tex_info.Filename = "european_fnt.tga";
    tex_info.MinFilter = base::opengl::TextureMinFilters::LINEAR;
    tex_info.GenerateMipmap = true;

    texture_ = new base::opengl::Texture;
    texture_->Generate(tex_info);

    std::string status;
    program_ = base::opengl::Program::Create("shader.shader", status);
    if (program_ == NULL) {
        std::cout << "Error at creating shader program: " << status << std::endl;
        assert(false);
    }
    program_->Bind();
    
    base::math::Vector4 cc(0, 0, 0, 1);

    std::vector<base::opengl::Mesh*> mesh_list 
        = base::opengl::load_md3_se("european_fnt_v2.md3");
    for (u32 i=0; i<mesh_list.size(); i++) {
        base::opengl::Mesh* m = mesh_list[i];
        base::opengl::VertexBuffer* vb = new base::opengl::VertexBuffer;
        vb->SetData(m->vertexes, m->num_vertexes, m->faces, m->num_faces);
        mesh_.push_back(vb);
        delete m;
    }
}

SDLDemo::~SDLDemo() {
    for (u32 i=0; i<mesh_.size(); i++)
        delete mesh_[i];
    delete buffer_;
    delete texture_;
    delete program_;
}

void SDLDemo::OnFrame() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);

    i32 pos = program_->get_attributes()["position"].Location;
    i32 tex = program_->get_attributes()["tex"].Location;
    i32 color = program_->get_attributes()["color"].Location;
    i32 normal = program_->get_attributes()["n"].Location;

    base::opengl::AttributeBinding binding;
    binding.push_back(std::make_pair(base::opengl::VertexAttrs::tagPosition, pos));
    binding.push_back(std::make_pair(base::opengl::VertexAttrs::tagTexture, tex));
//    binding.push_back(std::make_pair(base::opengl::VertexAttrs::tagColor, color));
    binding.push_back(std::make_pair(base::opengl::VertexAttrs::tagNormal, normal));

    base::generic_param<base::opengl::Texture*> p(texture_);
    program_->set_uniform("diffuse", p);

    base::generic_param<base::math::Matrix4> proj(projection_);
    program_->set_uniform("projection_matrix", proj);

    base::generic_param<base::math::Matrix4> modv(modelview_);
    program_->set_uniform("modelview_matrix", modv);

    for (u32 i=0; i<mesh_.size(); i++) {
        mesh_[i]->Draw(binding);
    }

    assert(glGetError() == GL_NO_ERROR);

    SDLApp::OnFrame();
}

int main(int argc, char *argv[])
{
    SDLDemo app(512, 512);
    app.Run();
 
    return 0;
}