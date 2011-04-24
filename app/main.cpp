#include "app/demo.h"
#include <GL/glut.h>

int main(int argc, char** argv) {
    {
        GlutSampleWindow app(800, 600);
        app.Run();
    }
    return 0;
}
