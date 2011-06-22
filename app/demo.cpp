#include "app/demo.h"

#include <string>
#include <iostream>

#include "GL/glew.h"
#include <GL/gl.h>

#include <GL/glut.h>

#include <fstream>

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
    base::math::Matrix4 m(2 / width, 0, 0, -(right+left)/width,
        0, 2 / height, 0, -(top+bottom)/height,
        0, 0, -2 / range, -(_far+_near)/range,
        0, 0, 0, -1);
    m.Transpose();
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

//    glMatrixMode(GL_PROJECTION);
    projection_ = GetOrtho(-4.0, 4.0, -4.0, 4.0, -4.0, 100.0);

//    glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();
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

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

GlutSampleWindow::~GlutSampleWindow() {
    delete texture_;
}

void GlutSampleWindow::OnDisplay(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

/*    float ambient_and_diffuse [] = { 0.3f, 0.3f, 0.3f, 1.0f };
    float specular[] = { 0.8f, 0.8f, 0.8f, 1.f };
    float emission[] = { 0.f, 0.f, 0.f, 1.f };

    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ambient_and_diffuse );
    glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, specular );
    glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, emission );
    glMateriali( GL_FRONT_AND_BACK, GL_SHININESS, 64 );*/

//    glBindTexture(GL_TEXTURE_2D, texture_->id());

//    glutSolidTeapot(1.f);
    i32 pos = program_->get_attributes()["position"].Location;
    i32 tex = program_->get_attributes()["tex"].Location;
    i32 color = program_->get_attributes()["color"].Location;

    base::generic_param<base::opengl::Texture*> p;
    p.Value = texture_;
    program_->set_uniform("diffuse", p);

    base::generic_param<base::math::Matrix4> proj;
    proj.Value = projection_;
    program_->set_uniform("projection_matrix", proj);

    base::generic_param<base::math::Matrix4> modv;
    modv.Value = modelview_;
    program_->set_uniform("modelview_matrix", modv);

    glBegin(GL_QUADS);
        glVertexAttrib2f(pos, -0.5f, 0.5f);
        glVertexAttrib2f(tex, 0.f, 0.f);
        glVertexAttrib4f(color, 0.f, 0.0f, 1.f, 1.f);

        glVertexAttrib2f(pos, 0.5f, 0.5f);
        glVertexAttrib2f(tex, 1.f, 0.f);
        glVertexAttrib4f(color, 0.f, 1.0f, 1.f, 1.f);

        glVertexAttrib2f(pos, 0.5f, -0.5f);
        glVertexAttrib2f(tex, 1.f, 1.f);
        glVertexAttrib4f(color, 0.f, 0.0f, 1.f, 1.f);

        glVertexAttrib2f(pos, -0.5f, -0.5f);
        glVertexAttrib2f(tex, 0.f, 1.f);
        glVertexAttrib4f(color, 1.f, 0.0f, 1.f, 1.f);
    glEnd();

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

