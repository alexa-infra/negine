/**
 * \file
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
 #include "app/glutapp.h"

#include <iostream>
#include <string>

#ifdef OS_WIN
# include <windows.h>
#endif

#include "GL/glew.h"
#include "GL/glut.h"
#include "GL/freeglut_ext.h"

#include <assert.h>

#ifdef OS_WIN
#include "GL/wglew.h"
#elif defined(OS_LINUX)
#include "GL/glxew.h"
#endif

GlutWindow* GlutWindow::window_ = NULL;

GlutWindow::GlutWindow()
: run_(true)
, width_(640)
, height_(480)
, capture_(false)
{
    int dummy_argc = 1;
    const char *dummy_argv[] = { "", NULL };
    glutInit(&dummy_argc, const_cast<char**>(dummy_argv));

    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(width_, height_);
    glutInitWindowPosition(100, 100);

//    glutInitContextVersion(3, 3);
//    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);
//    glutInitContextProfile(GLUT_CORE_PROFILE);

    window_ = this;
    window_id_ = glutCreateWindow("GlutWindow");

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

    glutReshapeFunc(OnReshapeProc);
    glutKeyboardFunc(OnKeyboardProc);
    glutMouseFunc(OnMouseProc);
    glutMotionFunc(OnMotionProc);
    glutPassiveMotionFunc(OnPassiveMotionProc);
    glutVisibilityFunc(OnVisibilityProc);
    glutIdleFunc(OnIdleProc);
    glutCloseFunc(OnCloseProc);

    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
#ifdef OS_WIN
    wglSwapInterval(1);
#elif defined(OS_LINUX)
    glXSwapIntervalSGI(1);
#endif
}

GlutWindow::~GlutWindow() {
    glutDestroyWindow(window_id_);
    window_ = NULL;
    glutExit();
}

void GlutWindow::Run() {
    glutMainLoopEvent();
    while (run_) {
        OnFrame();
        glutMainLoopEvent();
    }
}

void GlutWindow::OnCloseProc() {
    window_->run_ = false;
    window_->OnClose();
    glutLeaveMainLoop();
}

void GlutWindow::OnFrame(void) {
    glutSwapBuffers();
}

void GlutWindow::OnMouse(i32 button, i32 state, i32 x, i32 y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_UP && capture_) {
            capture_ = false;
            glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
        } else if (state == GLUT_DOWN && !capture_) {
            capture_ = true;
            glutSetCursor(GLUT_CURSOR_NONE);
        }
    }
}

void GlutWindow::OnMotion(i32 x, i32 y, i32 dx, i32 dy) {
    if (capture_) {
        glutWarpPointer(width_ / 2, height_ / 2);
    }
}

void GlutWindow::OnReshape(i32 width, i32 height) {
    width_ = width;
    height_ = height;
}