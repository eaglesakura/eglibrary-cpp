/*
 * GLContextUtil.hpp
 *
 *  Created on: 2014/08/23
 *      Author: eaglesakura
 */

#ifndef es_graphics_gl_GLCONTEXTUTIL_HPP_
#define es_graphics_gl_GLCONTEXTUTIL_HPP_

#include    "es/Graphics.hpp"

namespace es {
class GLContextUtil {
    GLContextUtil();
    ~GLContextUtil();
public:
    /**
     * 関連付けられたスレッドのglGetErrorを取り出す
     */
    static GLenum printError(const char* __file__, const int __line__) {
        const GLenum error = glGetError();
#define __prv__LOG_GL(err) case err: eslog_from(__file__, __line__, #err); break;
        switch (error) {
        __prv__LOG_GL(GL_INVALID_ENUM)
        __prv__LOG_GL(GL_INVALID_VALUE)
        __prv__LOG_GL(GL_INVALID_OPERATION)
        __prv__LOG_GL(GL_OUT_OF_MEMORY)
        __prv__LOG_GL(GL_INVALID_FRAMEBUFFER_OPERATION)
        }
#undef  __prv__LOG_GL
        return error;

    }
};
}

#if defined(DEBUG) && !defined(NO_GL_ASSERT)
#define     assert_gl(...)    { assert(::es::GLContextUtil::printError(__FILE__, __LINE__) == GL_NO_ERROR); }
#else
// release
#define     assert_gl(...)    { }
#endif

#endif /* GLCONTEXTUTIL_HPP_ */
