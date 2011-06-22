#include "app/glutapp.h"

#include <iostream>
#include <string>

#ifdef OS_WIN
# include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glut.h>

GlutWindow* GlutWindow::window_ = NULL;

GlutWindow::GlutWindow(u32 flags, i32 width/* = 640*/, i32 height/* = 480*/) {
    int dummy_argc = 1;
    char *dummy_argv[] = { "", NULL };
    glutInit(&dummy_argc, dummy_argv);

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

