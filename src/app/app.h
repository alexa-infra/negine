#pragma once

#if defined(SDL_ENGINE)
#include "app/sdlapp.h"
typedef SDLApp Application;
#elif defined(GLUT_ENGINE)
#include "app/glutapp.h"
typedef GlutWindow Application;
#else
#error Window system is not defined
#endif