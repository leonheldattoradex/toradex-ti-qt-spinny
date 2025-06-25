#pragma once

#ifdef USE_DESKTOP_GL
    #undef USE_OPENGL_ES
#else
    #define USE_OPENGL_ES
#endif
