#pragma once

#include    "es/eglibrary.hpp"

// include gl
#if defined(BUILD_MacOSX)

#include    <OpenGL/gl3.h>
#include    <OpenGL/gl3ext.h>

#ifndef GL_LUMINANCE
#define GL_LUMINANCE                      0x1909
#endif

#ifndef GL_HALF_FLOAT_OES
#define GL_HALF_FLOAT_OES                 0x8D61
#endif

#ifndef GL_DEPTH_COMPONENT32_OES
#define GL_DEPTH_COMPONENT32_OES          0x81A7
#endif

#ifndef GL_DEPTH_COMPONENT24_OES
#define GL_DEPTH_COMPONENT24_OES          0x81A6
#endif

#ifndef GL_DEPTH24_STENCIL8_OES
#define GL_DEPTH24_STENCIL8_OES           0x88F0
#endif

#elif defined(BUILD_Android)

#define __gl2_h_    /* include guard */

#include    <GLES3/gl31.h>
#include    <GLES3/gl3ext.h>
#include    <GLES2/gl2ext.h>
#include    <EGL/egl.h>
#include    <EGL/eglext.h>

#elif defined(BUILD_iOS)
#import     <OpenGLES/ES2/gl.h>
#import     <OpenGLES/ES2/glext.h>
#endif

#include    "es/math/Math.hpp"
#include    "es/graphics/GPU.h"
