#pragma once

#include "base/types.h"

#if defined OS_WIN
    #if !defined APIENTRY
        #define APIENTRY __stdcall
        #define _REMOVE_APIENTRY_DEF
    #endif
#endif

#include "opengl/GL/glcorearb.h"

#ifdef _REMOVE_APIENTRY_DEF
    #undef _REMOVE_APIENTRY_DEF
    #undef APIENTRY
#endif

#if !defined GL_TEXTURE_MAX_ANISOTROPY_EXT
    #define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
#endif
