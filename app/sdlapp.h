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
    bool run_;
    bool capture_;
    u32 width_;
    u32 height_;
    SDL_TimerID timer;
    
public:
    SDLApp(u32 width, u32 height);
    virtual ~SDLApp();
    void Run();
protected:
    virtual void OnFrame();
    virtual void OnMotion(f32 dx, f32 dy);
private:
    static u32 GameLoopTimer(u32 interval, void* param);
};