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

#include "render/glcontext.h"
#include "base/log.h"

#include "engine/engine.h"

namespace base {

using namespace opengl;

SDLApp::SDLApp()
    : mainwindow_( NULL )
    , run_( true )
    , capture_( false )
    , width_( 640 )
    , height_( 480 )
{
    Engine::init(GL);

    LOG("start");
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        ERR("Unable to init SDL: %s", SDL_GetError());
    }

    // my personal configuration, temporary
    #ifdef OS_MAC
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 2 );
    #elif defined(OS_WIN)
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
    #elif defined(OS_LINUX)
    #endif
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
    if (!maincontext_) {
        ERR("SDL error: %s", SDL_GetError());
    }

    GL.init();

    const GLubyte* version = GL.GetString( GL_VERSION );
    LOG("Current OpenGL version: %s", version);

    GL_ASSERT(GL);

    //SDL_GL_SetSwapInterval( 1 );
}

SDLApp::~SDLApp()
{
    SDL_GL_DeleteContext( maincontext_ );
    SDL_DestroyWindow( mainwindow_ );
    SDL_Quit();
    LOG("quit");

    Engine::shutdown();
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
}

void SDLApp::OnMotion( i32 x, i32 y, i32 dx, i32 dy )
{
}

void SDLApp::OnReshape( i32 width, i32 height )
{
}

void SDLApp::OnKeyboardDown( u8 key )
{   
}

void SDLApp::OnKeyboardUp( u8 key )
{   
}

} // namespace base
