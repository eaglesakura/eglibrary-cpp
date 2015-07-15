#ifndef es_glkit_EXTERNALIMAGETEXTURE_HPP_
#define es_glkit_EXTERNALIMAGETEXTURE_HPP_

#include    "es/OpenGL.hpp"
#include    "es_glkit_PreviewSurfaceTexture.h"

namespace es {

class ExternalImageTexture: public Texture {
    /**
     * 焼き込み用サーフェイス
     */
    ::std::shared_ptr<es::glkit::PreviewSurfaceTexture> surfaceTexture;

    /**
     * テクスチャのレンダリング準備ができたらtrue
     */
    bool renderingEnable;

    /**
     * UV操作用行列
     */
    Matrix4x4 uvMatrix;

    int updatedNum;
public:
    ExternalImageTexture() :
            Texture() {
        // external_oes必須
        setBindTarget(GL_TEXTURE_EXTERNAL_OES);
        renderingEnable = false;
        updatedNum = 0;
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
    virtual void setPreviewSurfaceTexture(::std::shared_ptr<es::glkit::PreviewSurfaceTexture> surface) {
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

//        eslog("matrix[%.2f, %.2f, %.2f, %.2f]", uvMatrix.m[0][0], uvMatrix.m[0][1], uvMatrix.m[0][2], uvMatrix.m[0][3]);
//        eslog("matrix[%.2f, %.2f, %.2f, %.2f]", uvMatrix.m[1][0], uvMatrix.m[1][1], uvMatrix.m[1][2], uvMatrix.m[1][3]);
//        eslog("matrix[%.2f, %.2f, %.2f, %.2f]", uvMatrix.m[2][0], uvMatrix.m[2][1], uvMatrix.m[2][2], uvMatrix.m[2][3]);
//        eslog("matrix[%.2f, %.2f, %.2f, %.2f]", uvMatrix.m[3][0], uvMatrix.m[3][1], uvMatrix.m[3][2], uvMatrix.m[3][3]);

        // 正常に行列が取得できるか、ある程度の計算回数を過ぎたらもう問題ないだろう
        renderingEnable = (!uvMatrix.isIdentity(0.00001f) || updatedNum > 30);
        ++updatedNum;
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

typedef ::std::shared_ptr<ExternalImageTexture> MExternalImageTexture;

}

#endif /* EXTERNALIMAGETEXTURE_HPP_ */
