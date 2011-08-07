/**
 * @author  Alexey Vasilyev <alexa.infra@gmail.com>
 * @section LICENSE
 * 
 * This file is part of Negine (https://alexadotlife.com/hg/negine)
 * Licensed under LGPLv3 (or GPLv3) – see LICENSE file for details
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

GlutWindow* GlutWindow::window_ = NULL;

GlutWindow::GlutWindow(u32 flags, i32 width/* = 640*/, i32 height/* = 480*/)
: is_closed_(false)
{
    int dummy_argc = 1;
    const char *dummy_argv[] = { "", NULL };
    glutInit(&dummy_argc, const_cast<char**>(dummy_argv));

    glutInitDisplayMode(flags);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(100, 100);

//    glutInitContextVersion(2, 0);
//    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);
//    glutInitContextProfile(GLUT_CORE_PROFILE);

    window_ = this;
    window_id_ = glutCreateWindow("GlutWindow");

    glutDisplayFunc(OnDisplayProc);
    glutReshapeFunc(OnReshapeProc);
    glutKeyboardFunc(OnKeyboardProc);
    glutMouseFunc(OnMouseProc);
    glutMotionFunc(OnMotionProc);
    glutPassiveMotionFunc(OnPassiveMotionProc);
    glutVisibilityFunc(OnVisibilityProc);
    glutIdleFunc(OnIdleProc);
    glutCloseFunc(OnCloseProc);

    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

    timer_.Reset();
    glutTimerFunc(1000 / 60, OnTimerProc, 0);
}

GlutWindow::~GlutWindow() {
    glutDestroyWindow(window_id_);
    window_ = NULL;
    glutExit();
}

void GlutWindow::Run() {
    while (!is_closed_) {
        glutMainLoopEvent();
    }
}

void GlutWindow::OnTimerCallback(u32 value) {
    if (!window_->is_closed_)
        OnDisplay();
    f32 elapsed = timer_.Elapsed();
    if (elapsed > 1000 / 60)
        elapsed = 0;
    else
        elapsed = 1000 / 60 - elapsed;
    glutTimerFunc(1000 / 60, OnTimerProc, value);
}

void GlutWindow::OnCloseProc() {
    window_->is_closed_ = true;
    glutLeaveMainLoop();
    window_->OnClose();
}
