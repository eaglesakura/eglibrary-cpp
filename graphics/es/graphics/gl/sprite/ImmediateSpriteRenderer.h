#pragma once

#include <es/asset/AssetManager.h>
#include "es/eglibrary.hpp"
#include "es/graphics/common/2d/ISpriteRenderingCallback.h"
#include "es/graphics/gl/context/DeviceContext.h"
#include "es/graphics/gl/resource/ShaderProgram.h"
#include "es/graphics/gl/sprite/Quad.h"

namespace es {

/**
 * 逐次レンダリングするシンプルなレンダラー
 */
class ImmediateSpriteRenderer : public Object, public ISpriteRenderingCallback {
public:
    ImmediateSpriteRenderer();

    /**
     * 初期化を行う
     */
    virtual void initialize(std::shared_ptr<DeviceContext> context, std::shared_ptr<AssetManager> assets);

    /**
     * レンダリングスレッドのContextと関連付ける
     */
    void setDeviceContext(std::shared_ptr<DeviceContext> context);


    virtual std::shared_ptr<RenderingContext> newRenderingContext(SpriteRenderer *sender) const override;

    virtual void beginRendering(SpriteRenderer *sender) override;

    virtual bool requestRendering(SpriteRenderer *sender, const std::shared_ptr<RenderingContext> &context) override;

    virtual void endRendering(SpriteRenderer *sender) override;

    virtual ~ImmediateSpriteRenderer() = default;

private:
    sp<DeviceContext> context;

    sp<ShaderProgram> shader;

    sp<Quad> quad;
};


}
