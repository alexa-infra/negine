/**
 * \file
 * \brief       SDL application
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#pragma once

#include "base/types.h"

struct SDL_Window;

namespace base {

class SDLApp
{
protected:
    SDL_Window* mainwindow_;     //!< window handle
    void* maincontext_;          //!< GL context
    bool run_;
    bool capture_;
    u32 width_;
    u32 height_;

public:
    SDLApp();
    virtual ~SDLApp();
    void Run();
protected:
    virtual void OnFrame();
    virtual void OnMotion( i32 x, i32 y, i32 dx, i32 dy );
    virtual void OnReshape( i32 width, i32 height );
    virtual void OnKeyboardDown( u8 key ) {}
    virtual void OnKeyboardUp( u8 key ) {}
    void Pump();
};

} // namespace base
