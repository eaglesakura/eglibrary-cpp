#pragma once

#include "es/eglibrary.hpp"

#include "es/graphics/common/2d/ITexture2D.h"
#include "es/graphics/common/2d/IDisplayTransfer2D.h"
#include "es/graphics/common/2d/ISpriteRenderingCallback.h"

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

    virtual ~SpriteRenderer() = default;

protected:
    /**
     * レンダリング本体はコールバッククラスに委譲する
     */
    sp<ISpriteRenderingCallback> callback;

    /**
     * レンダリング用コンテキスト
     */
    sp<ISpriteRenderingCallback::RenderingContext> renderContext;

    /**
     * ディスプレイ座標変換は別途クラスに委譲する
     */
    sp<IDisplayTransfer2D> display;
};


}
