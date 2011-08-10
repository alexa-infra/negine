/**
 * @author  Alexey Vasilyev <alexa.infra@gmail.com>
 * @section LICENSE
 * 
 * This file is part of Negine (https://alexadotlife.com/hg/negine)
 * Licensed under LGPLv3 (or GPLv3) - see LICENSE file for details
 **/
#pragma once

#include "base/types.h"

#ifdef OS_WIN
#  define SDL_MAIN_HANDLED
#endif

#include <SDL/SDL.h>

const i32 RUN_GAME_LOOP = 1;

class SDLApp {
protected:
    SDL_Window *mainwindow;     //!< window handle
    SDL_GLContext maincontext;  //!< GL context
    SDL_TimerID timer;
    bool run_;
    
public:
    SDLApp();
    ~SDLApp();

    void Run();

protected:
    static u32 GameLoopTimer(u32 interval, void* param);
    void HandleUserEvents(SDL_Event* event);
    void GameLoop();
};