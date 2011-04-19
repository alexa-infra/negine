#include "app/glutapp.h"

#include <iostream>
#include <string>

#include "GL/glew.h"
#include <GL/gl.h>

#include <GL/glut.h>

GlutWindow* GlutWindow::window_ = NULL;

GlutWindow::GlutWindow(u32 flags, i32 width/* = 640*/, i32 height/* = 480*/) {
    int dummy_argc = 0;
    glutInit(&dummy_argc, NULL);

    glutInitDisplayMode(flags);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(100, 100);

    window_ = this;
    window_id_ = glutCreateWindow("GlutWindow");

    glutDisplayFunc(OnDisplayProc);
    glutReshapeFunc(OnReshapeProc);
    glutKeyboardFunc(OnKeyboardProc);
    glutMouseFunc(OnMouseProc);
    glutMotionFunc(OnMotionProc);
    glutPassiveMotionFunc(OnPassiveMotionProc);
    glutVisibilityFunc(OnVisibilityProc);
}

GlutWindow::~GlutWindow() {
    glutDestroyWindow(window_id_);
    window_ = NULL;
}

void GlutWindow::Run() {
    glutMainLoop();
}

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
    glOrtho(-80.0, 80.0, -80.0, 80.0, -500.0, 500.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glRotatef(60, 1, 1, 1);
    glColor4f(1.0, 0.0, 0.0, 1.0);
}

GlutSampleWindow::~GlutSampleWindow() {
}

void GlutSampleWindow::OnDisplay(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glColor4f(1.0, 0.0, 0.0, 1.0);
    glutSolidTeapot(50);

    glutSwapBuffers();

    GLenum glstatus = glGetError();
    if (glstatus != GL_NO_ERROR) 
        std::cout << "GL Error: " << std::hex << glstatus << std::endl;
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

