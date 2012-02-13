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

class SDLApp {
protected:
    SDL_Window* mainwindow_;     //!< window handle
    SDL_GLContext maincontext_;  //!< GL context
    bool run_;
    bool capture_;
    u32 width_;
    u32 height_;
    
public:
    SDLApp(u32 width, u32 height);
    virtual ~SDLApp();
    void Run();
protected:
    virtual void OnFrame();
    virtual void OnMotion(f32 dx, f32 dy);
    void Pump();
};