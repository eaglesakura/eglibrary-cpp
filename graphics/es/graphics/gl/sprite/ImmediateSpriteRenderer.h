#pragma once

#include <es/asset/AssetManager.h>
#include "es/eglibrary.hpp"
#include "es/graphics/2d/ISpriteRenderingCallback.h"
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
     *
     * 失敗したらfalseを返す
     */
    virtual bool initialize(std::shared_ptr<DeviceContext> context, std::shared_ptr<AssetManager> assets);

    /**
     * レンダリングスレッドのContextと関連付ける
     */
    void setDeviceContext(std::shared_ptr<DeviceContext> context);

    virtual void beginRendering(SpriteRenderer *sender) override;


    virtual int requestRendering(SpriteRenderer *sender, const RenderingState *state, const uint numInstances, RenderingInstance *instanceArray) override;

    virtual void endRendering(SpriteRenderer *sender) override;

    virtual ~ImmediateSpriteRenderer() = default;

private:
    sp<DeviceContext> context;

    sp<ShaderProgram> shader;

    struct {
        /**
         * 回転角
         */
        FloatUniform rotate;

        /**
         * アスペクト比
         */
        FloatUniform aspect;

        /**
         * ポリゴンデータ用
         */
        Vector4fUniofrm poly_data;

        /**
         * ポリゴンUV用
         */
        Vector4fUniofrm poly_uv;

        /**
         * ブレンド色
         */
        ColorUniform color;

        /**
         * テクスチャ
         */
        TextureUniform texture;

        /**
         * 色描画とテクスチャ描画の切り替え
         */
        FloatUniform colorOnly;
    } uniform;

    sp<Quad> quad;
};


}
