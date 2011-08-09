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
    : mainwindow(NULL)
    , run_(true) 
{
    SDL_Init(SDL_INIT_VIDEO);
 
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
 
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
 
    mainwindow = SDL_CreateWindow(
        "SDL Demo",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        512, 512,
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

    timer = SDL_AddTimer(20, GameLoopTimer, this);
}

SDLApp::~SDLApp() {
    SDL_GL_DeleteContext(maincontext);
    SDL_DestroyWindow(mainwindow);
    SDL_Quit();
}

void SDLApp::Run() {
    SDL_Event event;
    
    while(run_ && (SDL_WaitEvent(&event))) {
        switch(event.type) {
            case SDL_USEREVENT:
                HandleUserEvents(&event);
                break;
            case SDL_QUIT:
                run_ = false;
                break;
            default:
                break;
        }
    }
}

void SDLApp::HandleUserEvents(SDL_Event* event) {
    switch (event->user.code) {
        case RUN_GAME_LOOP:
            GameLoop();
            break;
        default:
            break;
    }
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

void SDLApp::GameLoop() {
    glClearColor ( 1.0, 0.0, 0.0, 1.0 );
    glClear ( GL_COLOR_BUFFER_BIT );
    /* Swap our back buffer to the front */
    SDL_GL_SwapWindow(mainwindow);
}