#include "SpriteRenderer.h"
#include "es/graphics/math/GlmHelper.hpp"

namespace es {


SpriteRenderer::SpriteRenderer() {

}

void SpriteRenderer::setCallback(const std::shared_ptr<ISpriteRenderingCallback> callback) {
    assert(callback);

    this->callback = callback;
    this->renderContext = callback->newRenderingContext(this);
}

void SpriteRenderer::setDisplay(const std::shared_ptr<IDisplayTransfer2D> display) {
    assert(display);

    this->display = display;
}

void SpriteRenderer::renderingImage(std::shared_ptr<ITexture2D> texture, const float srcX, const float srcY, const float srcW, const float srcH, const float dstX, const float dstY, const float dstW, const float dstH, const float degree, const rgba32 rgba) {
    renderContext->texture = texture;
    renderContext->mode = ISpriteRenderingCallback::RenderingMode_QuadFill;
    renderContext->rotateRadian = glm::radians(degree);
    renderContext->color.rgba = rgba;

    // coord計算
    if (texture) {
        RectI16 area;
        Vector2i16 size;
        texture->getImageArea(&size, &area);


        RectF &srcCoord = renderContext->srcCoord;
        srcCoord.left = ((float) area.left + srcX) / (float) size.x;
        srcCoord.top = ((float) area.top + srcY) / (float) size.y;
        srcCoord.right = srcCoord.left + (srcW / (float) size.x);
        srcCoord.bottom = srcCoord.top + (srcH / (float) size.y);
    }

    // デバイス座標に変換
    {
        RectF &dstQuad = renderContext->dstQuad;
        const Vector2f leftTop = display->getSpritePositionToDevice(dstX, dstY);
        const Vector2f rightBottom = display->getSpritePositionToDevice(dstX + dstW, dstY + dstH);
        dstQuad.left = leftTop.x;
        dstQuad.top = leftTop.y;
        dstQuad.right = rightBottom.x;
        dstQuad.bottom = rightBottom.y;
    }

    callback->requestRendering(this, renderContext);
}

void SpriteRenderer::renderingRect(const float x, const float y, const float w, const float h, const rgba32 rgba) {
    renderingImage(sp<ITexture2D>(), 0, 0, 0, 0, x, y, w, h, 0, rgba);
}

void SpriteRenderer::begin() {
    assert(callback);
    assert(display);

    callback->beginRendering(this);
}

void SpriteRenderer::end() {
    callback->endRendering(this);
}

}