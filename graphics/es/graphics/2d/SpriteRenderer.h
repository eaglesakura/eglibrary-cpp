#pragma once

#include "es/protoground.hpp"

#include "es/graphics/resource/ITexture.hpp"
#include "es/graphics/2d/IDisplayTransfer2D.h"
#include "es/graphics/2d/ISpriteRenderingCallback.h"

namespace es {

/**
 * Spriteのレンダリング補助を行う。
 *
 * 実際のレンダリングは別なクラスに任せられる。
 */
class SpriteRenderer : public Object {
public:
    SpriteRenderer();

    void setCallback(const std::shared_ptr<ISpriteRenderingCallback> callback);

    void setDisplay(const std::shared_ptr<IDisplayTransfer2D> display);

    /**
     * 四角形描画を行う
     */
    void renderingRect(const float x, const float y, const float w, const float h, const rgba32 rgba);

    /**
     * 画像を描画する
     */
    void renderingImage(std::shared_ptr<ITexture> texture, const float srcX, const float srcY, const float srcW, const float srcH, const float dstX, const float dstY, const float dstW, const float dstH, const float degree, const rgba32 rgba);

    /**
     * レンダリングを開始する
     */
    virtual void begin();

    /**
     * レンダリングを終了する
     */
    virtual void end();

    virtual ~SpriteRenderer() = default;

protected:

    /**
     * レンダリング本体はコールバッククラスに委譲する
     */
    sp<ISpriteRenderingCallback> callback;

    /**
     * ディスプレイ座標変換は別途クラスに委譲する
     */
    sp<IDisplayTransfer2D> display;
};


}
