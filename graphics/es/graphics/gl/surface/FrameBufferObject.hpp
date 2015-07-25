#pragma once

#include "es/protoground.hpp"
#include    "es/OpenGL.hpp"

namespace es {

/**
 * フレームバッファ管理
 */
class FrameBufferObject : public GLObject, public IRenderingSurface {
public:
    FrameBufferObject();

    virtual ~FrameBufferObject();

    /**
     * サーフェイス幅を取得する
     */
    int getWidth() const override;

    /**
     * サーフェイス高さを取得する
     */
    int getHeight() const override;

    std::shared_ptr<Texture> getColorTexture() const;

    std::shared_ptr<Texture> getDepthTexture() const;

    /**
     * オフスクリーンテクスチャを生成する
     */
    virtual void createOffscreenTexture(MDeviceContext context, const uint width, const uint height, const PixelFormat_e colorFormat, const uint depthBits, const bool hasStencil);

    /**
     * フレームバッファの仮想サイズを指定する
     */
    virtual void setSize(const uint width, const uint height);

    virtual GLuint getFramebuffer() const;

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
    virtual void attach(const GLenum attachment, MTexture texture);

    /**
     * テクスチャをアタッチする
     */
    virtual void attach(const GLenum attachment, MRenderBufferObject buffer);

    /**
     * レンダリング用カラーバッファを確保する
     */
    virtual void allocColorBuffer(const PixelFormat_e fmt);

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
    virtual void allocDepthBuffer(const uint depthBits);

    /**
     * 深度テクスチャを確保する。
     * ただし、未サポート端末（Tegra3系等）があるため、完全な互換性は保証しない。
     */
    virtual bool allocDepthTexture(MDeviceContext context) {
        if (!GPU::isSupport(GPUExtension_Texture_Depth)) {
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
    virtual void attachCurrentThread();

    /**
     * フレームバッファが正常にであることを検証する
     */
    virtual void checkFramebufferStatus();

    virtual bool hasColor() const {
        return (bool) color.tex || (bool) color.buffer;
    }

    virtual bool hasDepth() const {
        return (bool) depth.tex || (bool) depth.buffer;
    }

    virtual bool hasStencil() const {
        return (bool) stencil.buffer;
    }

    virtual void dispose() override;
private:
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
};


}
