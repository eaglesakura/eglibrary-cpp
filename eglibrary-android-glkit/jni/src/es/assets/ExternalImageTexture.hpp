#ifndef es_glkit_EXTERNALIMAGETEXTURE_HPP_
#define es_glkit_EXTERNALIMAGETEXTURE_HPP_

#include    "es/OpenGL.hpp"
#include    "es_glkit_PreviewSurfaceTexture.h"

namespace es {

class ExternalImageTexture: public Texture {
    /**
     * 焼き込み用サーフェイス
     */
    std_shared_ptr<es::glkit::PreviewSurfaceTexture> surfaceTexture;

    /**
     * テクスチャのレンダリング準備ができたらtrue
     */
    bool renderingEnable;

    /**
     * UV操作用行列
     */
    Matrix4x4 uvMatrix;
public:
    ExternalImageTexture() :
            Texture() {
        // external_oes必須
        setBindTarget(GL_TEXTURE_EXTERNAL_OES);
        renderingEnable = false;
    }

    virtual ~ExternalImageTexture() {
    }

    /**
     * テクスチャの仮想サイズを指定する
     */
    virtual void setSize(const uint width, const uint height) {
        size.img_width = size.tex_width = width;
        size.img_height = size.tex_height = height;
    }

    /**
     * glBindTexgture(target, xxx)の内容を変更する
     */
    virtual void setBindTarget(const GLenum target) {
        assert(target == GL_TEXTURE_EXTERNAL_OES);
        this->target = target;
    }

    /**
     * テクスチャ用メモリを確保する
     */
    virtual void allocPixelMemory(const PixelFormat_e pixelFormat, const int miplevel, const int width, const int height) {
        // このメソッド呼び出しは行えない
        // このテクスチャは直接PreviewSurfaceTextureへと渡す
        assert(false);
    }

    /**
     * SDKのSurfaceTextureを渡す
     */
    virtual void setPreviewSurfaceTexture(std_shared_ptr<es::glkit::PreviewSurfaceTexture> surface) {
        this->surfaceTexture = surface;
    }

    /**
     * UV行列を取得する
     */
    const Matrix4x4& getUvMatrix() const {
        return uvMatrix;
    }

    /**
     * Rendering対象として使用できる場合true
     */
    virtual bool isRenderingEnable() const {
        return renderingEnable;
    }

    /**
     * サーフェイスの内容をテクスチャへ焼きこむ
     */
    virtual bool renderingToTexture() {
        if (!surfaceTexture) {
            // サーフェイスが設定されていない
            return false;
        }

        // 焼きこみを行う。
        // この時、成功可否は関係ない
        surfaceTexture->renderingToTexture();

        if (renderingEnable) {
            // 一度でも焼きこみに成功し、かつ行列も取得できているならもうRendering可能
            return true;
        }

        // 行列を取り出す
        jc::lang::object_wrapper jMatrix = surfaceTexture->getTextureMatrix();
        if (!jMatrix.hasObject()) {
            // 行列が取り出せなかったからまだRenderingは行えない
            return false;
        }

        eslog("matrix updated");
        JNIEnv *env = jMatrix.getEnv();
        float* matrixBuffer = (float*) env->GetDirectBufferAddress(jMatrix.getJobject());
        // バッファをコピーする
        memcpy(uvMatrix.m, matrixBuffer, sizeof(float) * 16);

        renderingEnable = true;
        return true;
    }

    /**
     * 開放処理を行う
     */
    virtual void dispose() {
        Texture::dispose();
        surfaceTexture.reset();
    }
};

typedef std_shared_ptr<ExternalImageTexture> MExternalImageTexture;

}

#endif /* EXTERNALIMAGETEXTURE_HPP_ */
