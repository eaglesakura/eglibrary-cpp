#ifndef es_graphics_gl_FRAMEBUFFEROBJECT_HPP_
#define es_graphics_gl_FRAMEBUFFEROBJECT_HPP_

#include    "es/OpenGL.hpp"

namespace es {

/**
 * フレームバッファ管理
 */
class FrameBufferObject: public GLObject, public IRenderingSurface {
    GLuint framebuffer;

    /**
     * レンダリング先のカラーバッファ
     */
    struct {
        MTexture tex;
        MRenderBufferObject buffer;
    } color;

    /**
     * レンダリング先の深度バッファ
     */
    struct {
        MTexture tex;
        MRenderBufferObject buffer;
    } depth;

    /**
     * レンダリング先のステンシルバッファ
     */
    struct {
        MRenderBufferObject buffer;
    } stencil;

    /**
     * バッファサイズ
     */
    Vector2i16 size;
public:
    FrameBufferObject() {
        glGenFramebuffers(1, &framebuffer);
        assert(framebuffer);
        assert_gl();
    }

    virtual ~FrameBufferObject() {
        this->dispose();
    }

    /**
     * サーフェイス幅を取得する
     */
    virtual int getWidth() const {
        return size.x;
    }

    /**
     * サーフェイス高さを取得する
     */
    virtual int getHeight() const {
        return size.y;
    }

    virtual MTexture getColorTexture() const {
        return color.tex;
    }

    virtual MTexture getDepthTexture() const {
        return depth.tex;
    }

    /**
     * オフスクリーンテクスチャを生成する
     */
    virtual void createOffscreenTexture(MDeviceContext context, const uint width, const uint height, const PixelFormat_e colorFormat, const uint depthBits, const bool hasStencil) {
        setSize(width, height);

        pushBind(context);
        {
            allocColorTexture(colorFormat, context);
            if (depthBits > 0) {
                allocDepthBuffer(depthBits);
            }
#ifdef DEBUG
            checkFramebufferStatus();
#endif
        }
        popUnbind(context);
    }

    /**
     * フレームバッファの仮想サイズを指定する
     */
    virtual void setSize(const uint width, const uint height) {
        size.set((int16_t) width, (int16_t) height);
    }

    virtual GLuint getFramebuffer() const {
        return framebuffer;
    }

    /**
     * 現在のステートをpushし、新たにフレームバッファを指定する
     */
    virtual void pushBind(MDeviceContext context) {
        MRenderState state = get_render(context);
        state->push();
        state->bindFramebuffer(framebuffer);
        state->viewport(0, 0, getWidth(), getHeight());
    }

    /**
     * 現在のステートをpopし、元の状態へ戻す
     */
    virtual void popUnbind(MDeviceContext context) {
        MRenderState state = get_render(context);
        state->pop();
    }

    /**
     * 利用開始する
     */
    virtual void bind(MDeviceContext context) {
        get_render(context)->bindFramebuffer(framebuffer);
    }

    /**
     * 利用を終了する
     */
    virtual void unbind(MDeviceContext context) {
        get_render(context)->bindFramebuffer(0);
    }

    /**
     * テクスチャをアタッチする
     *
     * この時点でFramebufferがバインドされていなければならない
     */
    virtual void attach(const GLenum attachment, MTexture texture) {
        glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture->getTexture(), 0);

        // アタッチメントの種類によって保存先を変更する
        switch (attachment) {
        case GL_COLOR_ATTACHMENT0:
            this->color.tex = texture;
            this->color.buffer.reset();
            break;
        case GL_DEPTH_ATTACHMENT:
            this->depth.tex = texture;
            this->depth.buffer.reset();
            break;
        default:
            assert(false);
            break;
        }
    }

    /**
     * テクスチャをアタッチする
     */
    virtual void attach(const GLenum attachment, MRenderBufferObject buffer) {
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, buffer->getRenderbuffer());
        assert_gl();

        // アタッチメントの種類によって保存先を変更する
        switch (attachment) {
        case GL_COLOR_ATTACHMENT0:
            this->color.tex.reset();
            this->color.buffer = buffer;
            break;
        case GL_DEPTH_ATTACHMENT:
            this->depth.tex.reset();
            this->depth.buffer = buffer;
            break;
        case GL_DEPTH_STENCIL_ATTACHMENT:
            this->depth.tex.reset();
            this->depth.buffer = buffer;
            this->stencil.buffer = buffer;
            break;
        case GL_STENCIL_ATTACHMENT:
            this->stencil.buffer = buffer;
            break;
        default:
            assert(false);
            break;
        }
    }

    /**
     * レンダリング用カラーバッファを確保する
     */
    virtual void allocColorBuffer(const PixelFormat_e fmt) {
        MRenderBufferObject buffer(new RenderBufferObject());
        buffer->setInternalFormat(Pixel::toGLPixelFormat(fmt));
        buffer->resize(getWidth(), getHeight());
        attach(GL_COLOR_ATTACHMENT0, buffer);
    }

    /**
     * レンダリング用のテクスチャを確保する
     */
    virtual void allocColorTexture(const PixelFormat_e fmt, MDeviceContext context) {
        MTexture texture(new Texture());
        texture->bind(context);
        texture->allocPixelMemory(fmt, 0, getWidth(), getHeight());
        texture->setFilter(GL_NEAREST, GL_NEAREST);
        texture->setWrapMode(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
        texture->unbind(context);
        attach(GL_COLOR_ATTACHMENT0, texture);
    }

    /**
     * レンダリング用深度バッファを確保する
     */
    virtual void allocDepthBuffer(const uint depthBits) {
        GLenum internalformat = 0;
        if (depthBits >= 32 && GPUCapacity::isSupport(GPUExtension_Renderbuffer_Depth32)) {
            internalformat = GL_DEPTH_COMPONENT32_OES;
            eslog("alloc depth req(%d) -> D(32bit)", depthBits);
        } else if (depthBits >= 24 && GPUCapacity::isSupport(GPUExtension_Renderbuffer_Depth24)) {
            internalformat = GL_DEPTH_COMPONENT24_OES;
            eslog("alloc depth req(%d) -> D(24bit)", depthBits);
        } else if (depthBits >= 24 && GPUCapacity::isSupport(GPUExtension_Renderbuffer_PackedDepth24Stencil8)) {
            internalformat = GL_DEPTH24_STENCIL8_OES;
            eslog("alloc depth req(%d) -> D(16bit) S(8bit)", depthBits);
        } else {
            internalformat = GL_DEPTH_COMPONENT16;
            eslog("alloc depth req(%d) -> D(16bit)", depthBits);
        }
        MRenderBufferObject buffer(new RenderBufferObject());
        buffer->setInternalFormat(internalformat);

        if (internalformat == GL_DEPTH24_STENCIL8_OES) {
            // has Stencil
            attach(GL_DEPTH_STENCIL_ATTACHMENT, buffer);
        } else {
            // depth
            attach(GL_DEPTH_ATTACHMENT, buffer);
        }
    }

    /**
     * 深度テクスチャを確保する。
     * ただし、未サポート端末（Tegra3系等）があるため、完全な互換性は保証しない。
     */
    virtual bool allocDepthTexture(MDeviceContext context) {
        if (!GPUCapacity::isSupport(GPUExtension_Texture_Depth)) {
            return false;
        }

        MTexture texture(new Texture());
        texture->bind(context);
        texture->allocPixelMemory(PixelFormat_Depth, 0, getWidth(), getHeight());
        texture->unbind(context);

        attach(GL_DEPTH_COMPONENT, texture);
        return true;
    }

    /**
     * 現在のContext（Thread）に対して再度アタッチする
     */
    virtual void attachCurrentThread() {
        if (color.tex) {
            attach(GL_COLOR_ATTACHMENT0, color.tex);
        } else if (color.buffer) {
            attach(GL_COLOR_ATTACHMENT0, color.buffer);
        }

        bool attachedStencil = false;
        if ((bool) depth.buffer && (depth.buffer == stencil.buffer)) {
            // depth & stencil
            attach(GL_DEPTH_STENCIL_ATTACHMENT, depth.buffer);
            attachedStencil = true;
        } else if (depth.buffer) {
            attach(GL_DEPTH_ATTACHMENT, depth.buffer);
        } else if (depth.tex) {
            attach(GL_DEPTH_ATTACHMENT, depth.tex);
        }

        if (!attachedStencil && stencil.buffer) {
            attach(GL_STENCIL_ATTACHMENT, stencil.buffer);
        }
    }

    /**
     * フレームバッファが正常にであることを検証する
     */
    virtual void checkFramebufferStatus() {
#ifdef DEBUG
        GLint check = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (check != GL_FRAMEBUFFER_COMPLETE) {
            eslog("glCheckFramebufferStatus(0x%x)", check);
        }
        assert(check == GL_FRAMEBUFFER_COMPLETE);
#endif
    }

    virtual bool hasColor() const {
        return (bool) color.tex || (bool) color.buffer;
    }

    virtual bool hasDepth() const {
        return (bool) depth.tex || (bool) depth.buffer;
    }

    virtual bool hasStencil() const {
        return (bool) stencil.buffer;
    }

    virtual void dispose() {
        if (!framebuffer) {
            return;
        }

        glDeleteFramebuffers(1, &framebuffer);
        framebuffer = 0;

        // 全バッファ解放
        color.tex.reset();
        color.buffer.reset();
        depth.tex.reset();
        depth.buffer.reset();
        stencil.buffer.reset();
    }
};

/**
 * managed
 */
typedef ::std::shared_ptr<FrameBufferObject> MFrameBufferObject;
}

#endif /* FRAMEBUFFEROBJECT_HPP_ */
