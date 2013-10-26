#include "sdlapp.h"

using namespace base;

class Demo : public SDLApp
{
public:
    Demo() {
    }
    virtual ~Demo() {
    }
protected:
    void OnFrame() {
        GL.ClearColor( 1.0f, 0.0f, 0.0f, 1.0f );
        GL.Clear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        GL_ASSERT(GL);
        SDLApp::OnFrame();
    }

    void OnMotion( i32 x, i32 y, i32 dx, i32 dy ) {
    }

    void OnKeyboardDown( u8 key ) {
    }
};

int main()
{
    Demo app;
    app.Run();
    return 0;
}
