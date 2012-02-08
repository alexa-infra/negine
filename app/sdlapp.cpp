/**
 * @author  Alexey Vasilyev <alexa.infra@gmail.com>
 * @section LICENSE
 * 
 * This file is part of Negine (https://alexadotlife.com/hg/negine)
 * Licensed under LGPLv3 (or GPLv3) - see LICENSE file for details
 **/
#include "app/sdlapp.h"

#include <iostream>
#include <string>
#include <assert.h>

#include "GL/glew.h"

SDLApp::SDLApp(u32 width, u32 height)
    : mainwindow(NULL)
    , run_(true)
    , width_(width)
    , height_(height)
    , capture_(false)
{
    SDL_Init(SDL_INIT_VIDEO);
 
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
 
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    mainwindow = SDL_CreateWindow(
        "SDL Demo",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width, height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
    );
    
    maincontext = SDL_GL_CreateContext(mainwindow);
 
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
    SDL_GL_SetSwapInterval(1);

    timer = SDL_AddTimer(50, GameLoopTimer, this);
}

SDLApp::~SDLApp() {
    SDL_GL_DeleteContext(maincontext);
    SDL_DestroyWindow(mainwindow);
    SDL_Quit();
}

void SDLApp::Run() {
    SDL_Event event;
    
    while(run_) {
        if (SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_USEREVENT:
                    if (event.user.code == RUN_GAME_LOOP)
                        OnFrame();
                    break;
                case SDL_QUIT:
                    run_ = false;
                    break;
                case SDL_MOUSEBUTTONUP:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        capture_ = false;
                        SDL_ShowCursor(1);
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        capture_ = true;
                        SDL_ShowCursor(0);
                    }
                    break;
                case SDL_MOUSEMOTION:
                    if (capture_) {
                        if (event.motion.x != width_ / 2. ||
                            event.motion.y != height_ / 2.) {
                            OnMotion((f32)event.motion.xrel, 
                                (f32)event.motion.yrel);
                            SDL_WarpMouseInWindow(mainwindow,
                                width_ / 2, height_ / 2);
                        }
                    }
                    break;
                default:
                    break;
            }
        }
    }
}

void SDLApp::OnFrame() {
    /* Swap our back buffer to the front */
    SDL_GL_SwapWindow(mainwindow);
}

void SDLApp::OnMotion(f32 dx, f32 dy) {
    
}

u32 SDLApp::GameLoopTimer(u32 interval, void* param) {
    SDL_Event event;

    event.type = SDL_USEREVENT;
    event.user.code = RUN_GAME_LOOP;
    event.user.data1 = 0;
    event.user.data2 = 0;

    SDL_PushEvent(&event);

    return interval;
}
