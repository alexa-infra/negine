/**
 * \file
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#include "glutapp.h"

#include <iostream>
#include <string>

#ifdef OS_WIN
# include <windows.h>
#endif

#include "render/glcontext.h"
#include "GL/glut.h"
#include "GL/freeglut_ext.h"

#include "render/statistics.h"

using namespace base;
GlutWindow* GlutWindow::window_ = NULL;

GlutWindow::GlutWindow()
    : run_( true )
    , width_( 640 )
    , height_( 480 )
    , capture_( false )
{
    int dummy_argc = 1;
    const char* dummy_argv[] = { "", NULL };
    glutInit( &dummy_argc, const_cast<char**>( dummy_argv ) );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE );
    glutInitWindowSize( width_, height_ );
    glutInitWindowPosition( 100, 100 );

    // my personal configuration, temporary
    #ifdef OS_WIN
        glutInitContextVersion(3, 1);
        glutInitContextFlags(/*GLUT_FORWARD_COMPATIBLE | */GLUT_DEBUG);
        glutInitContextProfile(GLUT_CORE_PROFILE);
    #elif defined(OS_MAC)
        glutInitContextVersion(3, 2);
        glutInitContextFlags(/*GLUT_FORWARD_COMPATIBLE | */GLUT_DEBUG);
        glutInitContextProfile(GLUT_CORE_PROFILE);
    #elif defined(OS_LINUX)
    #endif
    window_ = this;
    window_id_ = glutCreateWindow( "GlutWindow" );
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();

    if ( GLEW_OK != err ) {
        ERR("Unable to load GLEW: %s", glewGetErrorString( err ));
        abort();
    }

    if ( !GLEW_VERSION_3_3 ) {
        WARN("OpenGL 3.3 is not supported.");
    }

    GL_ASSERT();
    glutReshapeFunc( OnReshapeProc );
    glutKeyboardFunc( OnKeyboardProc );
    glutMouseFunc( OnMouseProc );
    glutMotionFunc( OnMotionProc );
    glutPassiveMotionFunc( OnPassiveMotionProc );
    glutVisibilityFunc( OnVisibilityProc );
    glutIdleFunc( OnIdleProc );
    glutCloseFunc( OnCloseProc );
    glutSetOption( GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION );
#ifdef OS_WIN
    wglSwapIntervalEXT( 1 );
#elif defined(OS_LINUX)
    glXSwapIntervalSGI( 1 );
#endif
    Stats::init();
}

GlutWindow::~GlutWindow()
{
    Stats::shutdown();
    glutDestroyWindow( window_id_ );
    window_ = NULL;
    glutExit();
}

void GlutWindow::Run()
{
    glutMainLoopEvent();

    while ( run_ ) {
        OnFrame();
        glutMainLoopEvent();
    }
}

void GlutWindow::OnCloseProc()
{
    window_->run_ = false;
    window_->OnClose();
    glutLeaveMainLoop();
}

void GlutWindow::OnFrame( void )
{
    glutSwapBuffers();
}

void GlutWindow::OnMouse( i32 button, i32 state, i32 x, i32 y )
{
    if ( button == GLUT_LEFT_BUTTON ) {
        if ( state == GLUT_UP && capture_ ) {
            capture_ = false;
            glutSetCursor( GLUT_CURSOR_RIGHT_ARROW );
        } else if ( state == GLUT_DOWN && !capture_ ) {
            capture_ = true;
            glutSetCursor( GLUT_CURSOR_NONE );
        }
    }
}

void GlutWindow::OnMotion( i32 x, i32 y, i32 dx, i32 dy )
{
    if ( capture_ ) {
        glutWarpPointer( width_ / 2, height_ / 2 );
    }
}

void GlutWindow::OnReshape( i32 width, i32 height )
{
    width_ = width;
    height_ = height;
}
