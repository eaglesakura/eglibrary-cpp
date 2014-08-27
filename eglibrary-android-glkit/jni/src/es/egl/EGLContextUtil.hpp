#ifndef es_egl_EGLCONTEXTUTIL_HPP_
#define es_egl_EGLCONTEXTUTIL_HPP_

#include    "es/GLKit.hpp"

namespace es {

class EGLContextUtil {
    EGLContextUtil();
    ~EGLContextUtil();
public:

    /**
     * エラーを出力する
     * GL_NOERROR以外だったらjctrueを返す
     */
    static EGLenum printError(const char* __file__, const int __line__) {
        const GLenum error = eglGetError();
#define LOG_EGL(err) case err: eslog_from(__file__, __line__, #err); break;

        switch (error) {
        LOG_EGL(EGL_NOT_INITIALIZED)
        LOG_EGL(EGL_BAD_ACCESS)
        LOG_EGL(EGL_BAD_ALLOC)
        LOG_EGL(EGL_BAD_ATTRIBUTE)
        LOG_EGL(EGL_BAD_CONFIG)
        LOG_EGL(EGL_BAD_CONTEXT)
        LOG_EGL(EGL_BAD_CURRENT_SURFACE)
        LOG_EGL(EGL_BAD_DISPLAY)
        LOG_EGL(EGL_BAD_MATCH)
        LOG_EGL(EGL_BAD_NATIVE_PIXMAP)
        LOG_EGL(EGL_BAD_NATIVE_WINDOW)
        LOG_EGL(EGL_BAD_PARAMETER)
        LOG_EGL(EGL_BAD_SURFACE)
        LOG_EGL(EGL_CONTEXT_LOST)
        }

        return error;
#undef LOG_EGL
    }
};

#if defined(DEBUG) && !defined(NO_EGL_ASSERT)
#define     assert_egl(...)    { assert(::es::EGLContextUtil::printEGLError(__FILE__, __LINE__) == EGL_SUCCESS); }
#else
// release
#define     assert_egl(...)    { }
#endif

}

#endif /* EGLCONTEXTUTIL_HPP_ */
