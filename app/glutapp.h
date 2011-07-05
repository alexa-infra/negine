#pragma once 

#include "base/types.h"
#include "base/timer.h"

class GlutWindow {
private:
    i32 window_id_;

    static GlutWindow* window_;

    static void OnDisplayProc(void) { 
        window_->OnDisplay(); 
    }
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
        window_->OnMotion(x, y); 
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
    static void OnTimerProc(int value) {
        window_->OnTimerCallback(value);
    }
    static void OnCloseProc();
public:
    GlutWindow(u32 flags, i32 width = 640, i32 height = 480); 
    virtual ~GlutWindow();
    void Run();
private:
    virtual void OnDisplay(void) {}
    virtual void OnReshape(i32 width, i32 height) {}
    virtual void OnKeyboard(u8 key, i32 x, i32 y) {}
    virtual void OnMouse(i32 button, i32 state, i32 x, i32 y) {}
    virtual void OnMotion(i32 x, i32 y) {}
    virtual void OnPassiveMotion(i32 x, i32 y) {}
    virtual void OnVisibility(i32 state) {}
    virtual void OnIdle(void) {}
    virtual void OnTimer() {}
    virtual void OnClose() {}
    void OnTimerCallback(u32 time);

    base::Timer timer_;
    bool is_closed_;

protected:
    f32 GetTimer() { return timer_.Reset(); }
};

