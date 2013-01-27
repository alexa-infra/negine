/**
 * \file
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#include "sdlapp.h"

#ifdef OS_WIN
#  define SDL_MAIN_HANDLED
#endif

#include <SDL.h>

#include <iostream>
#include <string>
#include <assert.h>

#include "renderer/glcontext.h"
#include "renderer/statistics.h"

SDLApp::SDLApp()
    : mainwindow_( NULL )
    , run_( true )
    , capture_( false )
    , width_( 640 )
    , height_( 480 )
{
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        std::cout << "Unable to init SDL: " << SDL_GetError() << std::endl;
        assert( false );
    }

    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
    // see SDL_GLprofile enumeration
    // e.g. SDL_GL_CONTEXT_PROFILE_ES2 for GLES
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
    // debug flag
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG );
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 24 );
    SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );

    mainwindow_ = SDL_CreateWindow(
                      "SDL Demo",
                      SDL_WINDOWPOS_CENTERED,
                      SDL_WINDOWPOS_CENTERED,
                      width_, height_,
                      SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
                  );
    maincontext_ = SDL_GL_CreateContext( mainwindow_ );
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();

    if ( GLEW_OK != err ) {
        std::cout << glewGetErrorString( err ) << std::endl;
        assert( false );
    }

    if ( !GLEW_VERSION_3_3 ) {
        std::cout << "OpenGL 3.3 is not supported." << std::endl;
        //    assert(false);
    }

    const GLubyte* version = glGetString( GL_VERSION );

    std::cout << "Current OpenGL version: " << version << std::endl;

    assert( glGetError() == GL_NO_ERROR );

    SDL_GL_SetSwapInterval( 1 );

    Stats::init();
}

SDLApp::~SDLApp()
{
    Stats::shutdown();
    SDL_GL_DeleteContext( maincontext_ );
    SDL_DestroyWindow( mainwindow_ );
    SDL_Quit();
}

void SDLApp::Pump()
{
    SDL_PumpEvents();
    SDL_Event event;

    while ( SDL_PollEvent( &event ) ) {
        switch( event.type ) {
        case SDL_QUIT:
            run_ = false;
            break;
        case SDL_MOUSEBUTTONUP:

            if ( event.button.button == SDL_BUTTON_LEFT ) {
                capture_ = false;
                SDL_ShowCursor( 1 );
            }

            break;
        case SDL_MOUSEBUTTONDOWN:

            if ( event.button.button == SDL_BUTTON_LEFT ) {
                capture_ = true;
                SDL_ShowCursor( 0 );
            }

            break;
        case SDL_MOUSEMOTION:

            if ( capture_ ) {
                if ( event.motion.x != width_ / 2. ||
                        event.motion.y != height_ / 2. ) {
                    OnMotion( event.motion.x,
                              event.motion.y,
                              event.motion.xrel,
                              event.motion.yrel );
                    SDL_WarpMouseInWindow( mainwindow_,
                                           width_ / 2, height_ / 2 );
                }
            }

            break;
        case SDL_KEYDOWN:
            OnKeyboardDown( ( u8 )event.key.keysym.sym );
            break;
        case SDL_KEYUP:
            OnKeyboardUp( ( u8 )event.key.keysym.sym );
            break;
        default:
            break;
        }
    }
}

void SDLApp::Run()
{
    Pump();

    while( run_ ) {
        OnFrame();
        Pump();
    }
}

void SDLApp::OnFrame()
{
    /* Swap our back buffer to the front */
    SDL_GL_SwapWindow( mainwindow_ );
    Stats::reset_frame();
}

void SDLApp::OnMotion( i32 x, i32 y, i32 dx, i32 dy )
{
}

void SDLApp::OnReshape( i32 width, i32 height )
{
}
