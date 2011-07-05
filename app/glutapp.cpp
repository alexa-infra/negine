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

GlutWindow::GlutWindow(u32 flags, i32 width/* = 640*/, i32 height/* = 480*/) {
    int dummy_argc = 1;
    const char *dummy_argv[] = { "", NULL };
    glutInit(&dummy_argc, const_cast<char**>(dummy_argv));

    glutInitDisplayMode(flags);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(100, 100);

    glutInitContextVersion(3, 3);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);
    glutInitContextProfile(GLUT_CORE_PROFILE);

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

    timer_.Reset();
    glutTimerFunc(1000 / 60, OnTimerProc, 0);
}

GlutWindow::~GlutWindow() {
    glutDestroyWindow(window_id_);
    window_ = NULL;
}

void GlutWindow::Run() {
    glutMainLoop();
}

void GlutWindow::OnTimerCallback(u32 value) {
    OnDisplay();
    f32 elapsed = timer_.Elapsed();
    if (elapsed > 1000 / 60)
        elapsed = 0;
    else
        elapsed = 1000 / 60 - elapsed;
    glutTimerFunc(1000 / 60, OnTimerProc, value);
}
