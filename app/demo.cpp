#include "app/demo.h"

#include <string>
#include <iostream>

#include "GL/glew.h"
#include <GL/gl.h>

#include <GL/glut.h>

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



base::math::Matrix4 GetOrtho(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far)
{
    f32 range = far - near;
    f32 width = right - left;
    f32 height = top - bottom;
    base::math::Matrix4 m(2 / width, 0,          0,          -(right+left)/width,
                          0,         2 / height, 0,          -(top+bottom)/height,
                          0,         0,          -2 / range, -(far+near)/range,
                          0,         0,          0,          1);
    return m;
}

GlutSampleWindow::GlutSampleWindow(i32 width, i32 height) 
    : GlutWindow(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE, 
                 width, height)
    , texture_(NULL)
    , program_(NULL)
{
    GLenum err = glewInit();
    if (GLEW_OK != err) 
        std::cout << glewGetErrorString(err) << std::endl;

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, width, height); 

    projection_ = GetOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 100.0);

    modelview_.SetIdentity();

//    glRotatef(60, 1, 1, 1);
//    glColor4f(1.0, 0.0, 0.0, 1.0);

    texture_ = new base::opengl::Texture;
    texture_->info().Filename = "texture.png";
    texture_->info().MinFilter = base::opengl::TextureMinFilters::LINEAR;
    texture_->info().GenerateMipmap = false;
    texture_->Generate();

    program_ = base::opengl::Program::Create(read_file("shader.vs"), read_file("shader.ps"));
    if (program_ != NULL)
        program_->Bind();

    sprite_ = new base::opengl::Sprite;

    base::opengl::Face* f = new base::opengl::Face[2];
    sprite_->init_faces(f, 0);

    base::opengl::Vertex* v = new base::opengl::Vertex[4];
    sprite_->init_vertex(v, 0);
    sprite_->fill_position(v, 0);

    buffer_ = new base::opengl::VertexBuffer(v, 4, f, 2);
    delete[] f;
    delete[] v;
}

GlutSampleWindow::~GlutSampleWindow() {
    delete sprite_;
    delete buffer_;
    delete texture_;
    delete program_;
}

void GlutSampleWindow::OnDisplay(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    i32 pos = program_->get_attributes()["position"].Location;
    i32 tex = program_->get_attributes()["tex"].Location;
    i32 color = program_->get_attributes()["color"].Location;

    base::opengl::AttributeBinding binding;
    binding.push_back(std::make_pair(base::opengl::VertexAttrs::tagPosition, pos));
    binding.push_back(std::make_pair(base::opengl::VertexAttrs::tagTexture, tex));
    binding.push_back(std::make_pair(base::opengl::VertexAttrs::tagColor, color));

    base::generic_param<base::opengl::Texture*> p;
    p.Value = texture_;
    program_->set_uniform("diffuse", p);

    base::generic_param<base::math::Matrix4> proj;
    proj.Value = projection_;
    program_->set_uniform("projection_matrix", proj);

    base::generic_param<base::math::Matrix4> modv;
    modv.Value = modelview_;
    program_->set_uniform("modelview_matrix", modv);

    base::opengl::Vertex* vertexes = buffer_->Lock();
    sprite_->rectange.angle += 0.1;
    if (sprite_->rectange.angle > 2 * base::math::pi)
        sprite_->rectange.angle -= 2 * base::math::pi;
    if (sprite_->rectange.angle < -2 * base::math::pi)
        sprite_->rectange.angle += 2 * base::math::pi;
    sprite_->fill_position(vertexes, 0);
    buffer_->Unlock();
    buffer_->Draw(binding);

    GLenum glstatus = glGetError();
    if (glstatus != GL_NO_ERROR) 
        std::cout << "GL Error: " << std::hex << glstatus << std::endl;
    glutSwapBuffers();
}

void GlutSampleWindow::OnReshape(i32 width, i32 height) {
    glViewport(0, 0, width, height); 
    OnDisplay();
}

void GlutSampleWindow::OnMotion(i32 x, i32 y) {
    modelview_.SetIdentity();
    modelview_.Rotate(base::math::Vector3(1, x, y), 60);
    OnDisplay();
}

void GlutSampleWindow::OnIdle(void) {
    OnDisplay();
}

