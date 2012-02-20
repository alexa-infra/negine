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

SDLApp::SDLApp()
    : mainwindow_(NULL)
    , run_(true)
    , width_(640)
    , height_(480)
    , capture_(false)
{
    SDL_Init(SDL_INIT_VIDEO);
 
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
 
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    mainwindow_ = SDL_CreateWindow(
        "SDL Demo",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width_, height_,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
    );
    
    maincontext_ = SDL_GL_CreateContext(mainwindow_);
 
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
}

SDLApp::~SDLApp() {
    SDL_GL_DeleteContext(maincontext_);
    SDL_DestroyWindow(mainwindow_);
    SDL_Quit();
}

void SDLApp::Pump() {
    SDL_PumpEvents();

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch(event.type) {
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
                        SDL_WarpMouseInWindow(mainwindow_,
                            width_ / 2, height_ / 2);
                    }
                }
                break;
            default:
                break;
        }
    }
}

void SDLApp::Run() {
    while(run_) {
        Pump();    
        OnFrame();
    }
}

void SDLApp::OnFrame() {
    /* Swap our back buffer to the front */
    SDL_GL_SwapWindow(mainwindow_);
}

void SDLApp::OnMotion(f32 dx, f32 dy) {
    
}

