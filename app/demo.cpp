#include "app/demo.h"

#include <string>
#include <iostream>

#include "GL/glew.h"
#include "GL/glut.h"

#include <fstream>
#include <utility>

std::string read_file(const std::string& filename) {
    int length;
    char* buffer;

    std::ifstream is;
    is.open(filename.c_str());
    if (!is.good())
        return "";

    // get length of file:
    is.seekg(0, std::ios::end);
    length = is.tellg();
    is.seekg(0, std::ios::beg);

    // allocate memory:
    buffer = new char[length];

    // read data as a block:
    is.read(buffer,length);
    is.close();

    std::string ret(buffer, length);
    delete[] buffer;
    return ret;
}



base::math::Matrix4 GetOrtho(f32 left, f32 right, f32 bottom, f32 top, f32 _near, f32 _far)
{
    f32 range = _far - _near;
    f32 width = right - left;
    f32 height = top - bottom;
    base::math::Matrix4 m(2 / width, 0,          0,          -(right+left)/width,
                          0,         2 / height, 0,          -(top+bottom)/height,
                          0,         0,          -2 / range, -(_far+_near)/range,
                          0,         0,          0,          1);
    return m;
}

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

    projection_ = GetOrtho(-150.0, 150.0, -150.0, 150.0, -500.0, 500.0);

    modelview_.SetIdentity();

    texture_ = new base::opengl::Texture;
    texture_->info().Filename = "european_fnt.tga";
    texture_->info().MinFilter = base::opengl::TextureMinFilters::LINEAR;
    texture_->info().GenerateMipmap = true;
    texture_->Generate();

    std::string status;
    program_ = base::opengl::Program::Create(read_file("shader.vs"), read_file("shader.ps"), status);
    if (program_ != NULL)
        program_->Bind();
    else
        std::cout << "Error at creating shader program: " << status << std::endl;

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
}

GlutSampleWindow::~GlutSampleWindow() {
    for (u32 i=0; i<mesh_.size(); i++)
        delete mesh_[i];
    delete sg_;
    delete buffer_;
    delete texture_;
    delete program_;
}

void GlutSampleWindow::OnDisplay(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);

    f32 frame_time = GetTimer();

    i32 pos = program_->get_attributes()["position"].Location;
    i32 tex = program_->get_attributes()["tex"].Location;
    i32 color = program_->get_attributes()["color"].Location;
    i32 normal = program_->get_attributes()["n"].Location;

    base::opengl::AttributeBinding binding;
    binding.push_back(std::make_pair(base::opengl::VertexAttrs::tagPosition, pos));
    binding.push_back(std::make_pair(base::opengl::VertexAttrs::tagTexture, tex));
//    binding.push_back(std::make_pair(base::opengl::VertexAttrs::tagColor, color));
    binding.push_back(std::make_pair(base::opengl::VertexAttrs::tagNormal, normal));

    base::generic_param<base::opengl::Texture*> p;
    p.Value = texture_;
    program_->set_uniform("diffuse", p);

    base::generic_param<base::math::Matrix4> proj;
    proj.Value = projection_;
    program_->set_uniform("projection_matrix", proj);

    base::generic_param<base::math::Matrix4> modv;
    modv.Value = modelview_;
    program_->set_uniform("modelview_matrix", modv);

    //sg_->Draw(binding);
    for (u32 i=0; i<mesh_.size(); i++) {
        mesh_[i]->Draw(binding);
    }

//    GLenum glstatus = glGetError();
//    if (glstatus != GL_NO_ERROR) 
//        std::cout << "GL Error: " << std::hex << glstatus << std::endl;
    assert(glGetError() == GL_NO_ERROR);
    glutSwapBuffers();
}

void GlutSampleWindow::OnReshape(i32 width, i32 height) {
    glViewport(0, 0, width, height); 
    OnDisplay();
}

void GlutSampleWindow::OnMotion(i32 x, i32 y) {
//    modelview_.SetIdentity();
    modelview_.Rotate(base::math::Vector3(0, 1, 1), 0.1f);
    OnDisplay();
}

