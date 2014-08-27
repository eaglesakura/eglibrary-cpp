/*
 * Graphics.hpp
 *
 *  Created on: 2014/08/23
 *      Author: eaglesakura
 */

#ifndef es_grahpics_GRAPHICS_HPP_
#define es_grahpics_GRAPHICS_HPP_

#include    "es/eglibrary.hpp"

// include gl
#ifdef BUILD_MacOSX
#include    <GL/gl3.h>

#else

#ifdef BUILD_Android

#define __gl2_h_    /* include guard */
#include    <GLES3/gl3.h>
#include    <GLES2/gl2ext.h>

#endif // BUILD_Android

#ifdef BUILD_iOS
#import     <OpenGLES/ES2/gl.h>
#import     <OpenGLES/ES2/glext.h>
#endif // BUILD_iOS
#endif

#include    "es/graphics/gl/context/GLContextUtil.hpp"

//#if defined(DEBUG) && !defined(NO_GL_ASSERT)
//#define     assert_gl(...)    { assert(::es::gl::GLState::printGLHasError(__FILE__, __LINE__) == jcfalse); }
//#else
//// release
//#define     assert_gl(...)    { }
//#endif

#endif /* GRAPHICS_HPP_ */
