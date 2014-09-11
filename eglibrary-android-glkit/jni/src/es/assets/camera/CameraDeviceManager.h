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
    CameraOrientationType_0 = glkit::CameraTextureRenderer::ORIENTATION_0,
    /**
     *
     */
    CameraOrientationType_90 = glkit::CameraTextureRenderer::ORIENTATION_90,
    /**
     *
     */
    CameraOrientationType_180 = glkit::CameraTextureRenderer::ORIENTATION_180,
    /**
     *
     */
    CameraOrientationType_270 = glkit::CameraTextureRenderer::ORIENTATION_270,
};

enum CameraAutofocusType_e {
    /**
     *
     */
    CameraAutofocusType_None = glkit::CameraTextureRenderer::AUTOFOCUS_NONE,
    /**
     *
     */
    CameraAutofocusType_Processing = glkit::CameraTextureRenderer::AUTOFOCUS_PROCESSING,
    /**
     *
     */
    CameraAutofocusType_Failed = glkit::CameraTextureRenderer::AUTOFOCUS_FAILED,
    /**
     *
     */
    CameraAutofocusType_Completed = glkit::CameraTextureRenderer::AUTOFOCUS_COMPLETED,
};

class CameraDeviceManager: public Object {
    /**
     * カメラ制御
     */
    std_shared_ptr<es::glkit::CameraTextureRenderer> camera;

    /**
     * プレビュー制御
     */
    std_shared_ptr<es::glkit::PreviewSurfaceTexture> previewSurface;

    /**
     * プレビュー対象テクスチャ
     */
    MExternalImageTexture previewTexture;
public:
    CameraDeviceManager(std_shared_ptr<es::glkit::CameraTextureRenderer> camera);
    virtual ~CameraDeviceManager();

    /**
     * カメラ制御クラスを取得する
     */
    virtual std_shared_ptr<es::glkit::CameraTextureRenderer> getCamera() const {
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
};

/**
 * カメラ制御
 */
typedef std_shared_ptr<CameraDeviceManager> MCameraDeviceManager;

}

#endif /* CAMERADEVICEMANAGER_HPP_ */
