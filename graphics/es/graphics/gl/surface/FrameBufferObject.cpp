#include "FrameBufferObject.hpp"

#include "es/internal/protoground-internal.hpp"

namespace es {

FrameBufferObject::FrameBufferObject() {
    glGenFramebuffers(1, &framebuffer);
    assert(framebuffer);
    assert_gl();
}

FrameBufferObject::~FrameBufferObject() {
    this->dispose();
}

int FrameBufferObject::getWidth() const {
    return size.x;
}

int FrameBufferObject::getHeight() const {
    return size.y;
}

std::shared_ptr<Texture> FrameBufferObject::getColorTexture() const {
    return color.tex;
}

std::shared_ptr<Texture> FrameBufferObject::getDepthTexture() const {
    return depth.tex;
}

void FrameBufferObject::createOffscreenTexture(MDeviceContext context, const uint width, const uint height, const PixelFormat_e colorFormat, const uint depthBits, const bool hasStencil) {
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


void FrameBufferObject::setSize(const uint width, const uint height) {
    size.set((int16_t) width, (int16_t) height);
}

GLuint FrameBufferObject::getFramebuffer() const {
    return framebuffer;
}

void FrameBufferObject::attach(const GLenum attachment, MTexture texture) {
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

void FrameBufferObject::attach(const GLenum attachment, MRenderBufferObject buffer) {
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

void FrameBufferObject::allocColorBuffer(const PixelFormat_e fmt) {
    MRenderBufferObject buffer(new RenderBufferObject());
    buffer->setInternalFormat(Pixel::toGLPixelFormat(fmt));
    buffer->resize(getWidth(), getHeight());
    attach(GL_COLOR_ATTACHMENT0, buffer);
}

void FrameBufferObject::checkFramebufferStatus() {
#ifdef DEBUG
    GLint check = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (check != GL_FRAMEBUFFER_COMPLETE) {
        eslog("glCheckFramebufferStatus(0x%x)", check);
    }
    assert(check == GL_FRAMEBUFFER_COMPLETE);
#endif
}

void FrameBufferObject::attachCurrentThread() {
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

void FrameBufferObject::allocDepthBuffer(const uint depthBits) {
    GLenum internalformat = 0;
    if (depthBits >= 32 && GPU::isSupport(GPUExtension_Renderbuffer_Depth32)) {
        internalformat = GL_DEPTH_COMPONENT32_OES;
        eslog("alloc depth req(%d) -> D(32bit)", depthBits);
    } else if (depthBits >= 24 && GPU::isSupport(GPUExtension_Renderbuffer_Depth24)) {
        internalformat = GL_DEPTH_COMPONENT24_OES;
        eslog("alloc depth req(%d) -> D(24bit)", depthBits);
    } else if (depthBits >= 24 && GPU::isSupport(GPUExtension_Renderbuffer_PackedDepth24Stencil8)) {
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

void FrameBufferObject::dispose() {
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

}
