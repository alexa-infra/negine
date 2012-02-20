/**
 * @author  Alexey Vasilyev <alexa.infra@gmail.com>
 * @section LICENSE
 * 
 * This file is part of Negine (https://alexadotlife.com/hg/negine)
 * Licensed under LGPLv3 (or GPLv3) - see LICENSE file for details
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

GlutWindow* GlutWindow::window_ = NULL;

GlutWindow::GlutWindow()
: is_closed_(false)
, width_(640)
, height_(480)
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
}

GlutWindow::~GlutWindow() {
    glutDestroyWindow(window_id_);
    window_ = NULL;
    glutExit();
}

void GlutWindow::Run() {
    while (!is_closed_) {
        glutMainLoopEvent();
        OnFrame();
    }
}

void GlutWindow::OnCloseProc() {
    window_->is_closed_ = true;
    glutLeaveMainLoop();
    window_->OnClose();
}

void GlutWindow::OnFrame(void) {
    glutSwapBuffers();
}