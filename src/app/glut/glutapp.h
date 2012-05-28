/**
 * \file
 * \brief       GLUT application
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
 #pragma once 

#include "base/types.h"
#include "base/timer.h"

class GlutWindow {
private:
    i32 window_id_;

    static GlutWindow* window_;

    static void OnReshapeProc(i32 width, i32 height) { 
        window_->OnReshape(width, height); 
    }
    static void OnKeyboardProc(u8 key, i32 x, i32 y) { 
        window_->OnKeyboard(key, x, y); 
    }
    static void OnMouseProc(i32 button, i32 state, i32 x, i32 y) { 
        window_->OnMouse(button, state, x, y); 
    }
    static void OnMotionProc(i32 x, i32 y) { 
        if (x == window_->width_ / 2 && y == window_->height_ / 2)
            return;

        static i32 oldx = x;
        static i32 oldy = y;
        window_->OnMotion(x, y, x - oldx, y - oldy);
        oldx = x;
        oldy = y;
    }
    static void OnPassiveMotionProc(i32 x, i32 y) { 
        window_->OnPassiveMotion(x ,y); 
    }
    static void OnVisibilityProc(i32 state) { 
        window_->OnVisibility(state); 
    }
    static void OnIdleProc() {
        window_->OnIdle();
    }
    static void OnCloseProc();
public:
    GlutWindow(); 
    virtual ~GlutWindow();
    void Run();
protected:
    virtual void OnFrame(void);
    virtual void OnReshape(i32 width, i32 height);
    virtual void OnKeyboard(u8 key, i32 x, i32 y) {}
    virtual void OnMouse(i32 button, i32 state, i32 x, i32 y);
    virtual void OnMotion(i32 x, i32 y, i32 dx, i32 dy);
    virtual void OnPassiveMotion(i32 x, i32 y) {}
    virtual void OnVisibility(i32 state) {}
    virtual void OnIdle(void) {}
    virtual void OnClose() {}

    bool run_;
    i32 width_;
    i32 height_;
    bool capture_;
};

