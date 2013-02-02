/**
 * \file
 * \brief       Depend on preprocessor defines GLUT or SDL windowing
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#pragma once

#if defined(SDL_ENGINE)
#include "app/sdl/sdlapp.h"
typedef base::SDLApp Application;
#elif defined(GLUT_ENGINE)
#include "app/glut/glutapp.h"
typedef GlutWindow Application;
#else
#error Window system is not defined
#endif
