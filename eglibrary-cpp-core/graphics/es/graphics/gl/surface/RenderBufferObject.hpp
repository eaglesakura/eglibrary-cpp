#ifndef es_graphics_gl_RENDERBUFFEROBJECT_HPP_
#define es_graphics_gl_RENDERBUFFEROBJECT_HPP_

#include    "es/OpenGL.hpp"

namespace es {

class RenderBufferObject: public GLObject {
    /**
     * バッファオブジェクト
     */
    GLuint renderbuffer;

    /**
     * バッファフォーマット
     */
    GLenum internalformat;

    /**
     * バッファのXYサイズ
     */
    Vector2i16 size;
public:
    RenderBufferObject() {
        glGenRenderbuffers(1, &renderbuffer);
        assert(renderbuffer);
        assert_gl();

        internalformat = 0;
    }
    virtual ~RenderBufferObject() {
    }

    virtual GLuint getRenderbuffer() const {
        return renderbuffer;
    }

    /**
     * フォーマット指定を行う
     */
    virtual void setInternalFormat(const GLenum fmt) {
        this->internalformat = fmt;
    }

    /**
     * サイズ変更を行う
     */
    virtual void resize(const uint width, const uint height) {
        glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
        {
            glRenderbufferStorage(GL_RENDERBUFFER, internalformat, width, height);
            assert_gl();
            size.x = width;
            size.y = height;
        }
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }

    /**
     * 解放処理を行う
     */
    virtual void dispose() {
        if (!renderbuffer) {
            return;
        }

        glDeleteRenderbuffers(1, &renderbuffer);
    }

    virtual uint getWidth() {
        return size.x;
    }

    virtual uint getHeight() {
        return size.y;
    }
};

typedef std_shared_ptr<RenderBufferObject> MRenderBufferObject;

}

#endif /* RENDERBUFFEROBJECT_HPP_ */
