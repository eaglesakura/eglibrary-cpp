#ifndef es_glkit_CAMERADEVICEMANAGER_H_
#define es_glkit_CAMERADEVICEMANAGER_H_

#include    "es/OpenGL.hpp"
#include    "es/assets/ExternalImageTexture.hpp"
#include    "es_glkit_CameraTextureRenderer.h"

namespace es {

enum CameraDeviceMode_e {
    /**
     *
     */
    CameraDeviceMode_MainCamera = glkit::CameraTextureRenderer::CAMERAMODE_MAIN,

    /**
     *
     */
    CameraDeviceMode_SubCamera = glkit::CameraTextureRenderer::CAMERAMODE_SUB,
};

enum CameraOrientationType_e {
    /**
     *
     */
    CameraOrientationType_0 = 0,
    /**
     *
     */
    CameraOrientationType_90 = 90,
    /**
     *
     */
    CameraOrientationType_180 = 180,
    /**
     *
     */
    CameraOrientationType_270 = 270,
};

class CameraDeviceManager: public Object {
    /**
     * カメラ制御
     */
    ::std::shared_ptr<es::glkit::CameraTextureRenderer> camera;

    /**
     * プレビュー制御
     */
    ::std::shared_ptr<es::glkit::PreviewSurfaceTexture> previewSurface;

    /**
     * プレビュー対象テクスチャ
     */
    MExternalImageTexture previewTexture;
public:
    CameraDeviceManager(::std::shared_ptr<es::glkit::CameraTextureRenderer> camera);
    virtual ~CameraDeviceManager();

    /**
     * カメラ制御クラスを取得する
     */
    virtual ::std::shared_ptr<es::glkit::CameraTextureRenderer> getCamera() const {
        return camera;
    }

    /**
     * プレビューテクスチャを取得する
     */
    virtual MExternalImageTexture getPreviewTexture() const {
        if ((bool) previewTexture && previewTexture->isRenderingEnable()) {
            return previewTexture;
        }
        return MExternalImageTexture();
    }

    /**
     * プレビューを開始する
     */
    virtual bool startPreview(MDeviceContext context);

    /**
     * 描画用の更新を行わせる
     */
    virtual void updatePreview(MDeviceContext context);

    /**
     * プレビューを停止する
     */
    virtual void stopPreview();

    /**
     * カメラを解放する
     */
    virtual void dispose();

    /**
     * プレビューサイズを得る
     */
    Vector2i16 getPreviewSize() const {
        return Vector2i16(camera->getPreviewWidth(), camera->getPreviewHeight());
    }
};

/**
 * カメラ制御
 */
typedef ::std::shared_ptr<CameraDeviceManager> MCameraDeviceManager;

}

#endif /* CAMERADEVICEMANAGER_HPP_ */
