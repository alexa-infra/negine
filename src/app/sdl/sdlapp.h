/**
 * \file
 * \brief       SDL application
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#pragma once

#include "base/types.h"
#include "render/glcontext.h"

struct SDL_Window;

namespace base {

class SDLApp
{
protected:
    SDL_Window* mainwindow_;     //!< window handle
    void* maincontext_;          //!< GL context
    opengl::DeviceContext GL;
    bool run_;
    bool capture_;
    u32 width_;
    u32 height_;

public:
    NEGINE_API SDLApp();
    NEGINE_API virtual ~SDLApp();
    NEGINE_API void Run();
    NEGINE_API opengl::DeviceContext& context() { return GL; }
protected:
    NEGINE_API virtual void OnFrame();
    NEGINE_API virtual void OnMotion( i32 x, i32 y, i32 dx, i32 dy );
    NEGINE_API virtual void OnReshape( i32 width, i32 height );
    NEGINE_API virtual void OnKeyboardDown( u8 key ) {}
    NEGINE_API virtual void OnKeyboardUp( u8 key ) {}
    NEGINE_API void Pump();
};

} // namespace base
