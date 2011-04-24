#include "app/demo.h"

#include <iostream>
#include <string>

#include "GL/glew.h"
#include <GL/gl.h>

#include <GL/glut.h>

GlutSampleWindow::GlutSampleWindow(i32 width, i32 height) 
    : GlutWindow(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE, 
                 width, height) 
{
    GLenum err = glewInit();
    if (GLEW_OK != err) 
        std::cout << glewGetErrorString(err) << std::endl;

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, width, height); 

    glMatrixMode(GL_PROJECTION);
    glOrtho(-2.0, 2.0, -2.0, 2.0, -2.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

//    glRotatef(60, 1, 1, 1);
//    glColor4f(1.0, 0.0, 0.0, 1.0);

    texture_ = new ext::opengl::Texture;
    texture_->info().Filename = "texture.png";
    texture_->info().MinFilter = ext::opengl::TextureMinFilters::LINEAR;
    texture_->info().GenerateMipmap = true;
    texture_->Generate();

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

    glBindTexture(GL_TEXTURE_2D, texture_->id());

    glutSolidTeapot(1.f);

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
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(60, 1, x, y);
    OnDisplay();
}

