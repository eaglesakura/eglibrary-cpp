#include    "CameraDeviceManager.h"

namespace es {

CameraDeviceManager::CameraDeviceManager(::std::shared_ptr<es::glkit::CameraTextureRenderer> camera) {
    this->camera = camera;
}

CameraDeviceManager::~CameraDeviceManager() {
    dispose();
}

/**
 * プレビューを開始する
 */
bool CameraDeviceManager::startPreview(MDeviceContext context) {
    this->previewTexture.reset(new ExternalImageTexture());

    previewTexture->bind(context);
    if (!camera->startPreview(previewTexture->getTexture())) {
        previewTexture->unbind(context);
        previewTexture.reset();
        return false;
    }

    // サーフェイスをラップする
    auto surface = camera->getPreviewSurface();
    this->previewSurface.reset(new es::glkit::PreviewSurfaceTexture(surface.globalRef()));
    this->previewTexture->setPreviewSurfaceTexture(previewSurface);

    // サーフェイスのサイズを指定する
    int previewWidth = camera->getPreviewWidth();
    int previewHeight = camera->getPreviewHeight();
    eslog("preview width(%d) height(%d)", previewWidth, previewHeight);
    previewTexture->setSize(previewWidth, previewHeight);

    return true;
}

/**
 * プレビューを停止する
 */
void CameraDeviceManager::stopPreview() {
    if (!camera) {
        return;
    }

    previewTexture.reset();
    previewSurface.reset();
    camera->stopPreview();
}

/**
 * 毎フレームに更新を行わせる
 */
void CameraDeviceManager::updatePreview(MDeviceContext context) {
    if (!context) {
        context = DeviceContext::current();
    }

    if (previewTexture) {
        previewTexture->bind(context);
        previewTexture->renderingToTexture();
        previewTexture->unbind(context);
    }
}

/**
 * カメラを解放する
 */
void CameraDeviceManager::dispose() {
    if (!camera) {
        return;
    }

    stopPreview();
    camera->disconnect();
    camera.reset();
}
}
