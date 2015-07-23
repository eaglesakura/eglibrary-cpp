#pragma once

#include "es/eglibrary.hpp"
#include "es/graphics/common/2d/ITexture2D.h"
#include <es/graphics/Color.hpp>

namespace es {

class SpriteRenderer;

class ISpriteRenderingCallback {
public:
    enum RenderingMode_e {
        /**
         * 四角形塗りつぶし描画を行う
         */
                RenderingMode_QuadFill,

        /**
         * 四角形アウトライン描画を行う
         */
                RenderingMode_QuadOutLine,

        /**
         * 線描画を行う
         */
                RenderingMode_Line,
    };

    struct RenderingContext {
        /**
         * レンダリングモードを指定する
         */
        RenderingMode_e mode = RenderingMode_QuadFill;

        /**
         * レンダリングテクスチャ
         *
         * カラーポリゴンのみを描画したい場合はnullptrが設定される
         */
        std::shared_ptr<ITexture2D> texture;

        /**
         * テクスチャのUV座標
         * ピクセルではなく、変換されたUV座標そのものが入力される。
         * textureがnullptrの場合は値は不定となる。
         */
        RectF srcCoord;

        /**
         * レンダリングサーフェイスのアスペクト比
         */
        float surfaceAspect = 1;

        /**
         * レンダリング先のディスプレイ座標（四角形描画）
         * ピクセルではなく、正規化座標が入力される
         */
        RectF dstQuad;

        /**
         * レンダリング先のディスプレイ座標（線描画）
         */
        struct {
            Vector2f start;
            Vector2f end;
        } dstLine;

        /**
         * 回転角が設定される
         */
        float rotateRadian = 0;

        /**
         * ブレンドしたい色が指定される
         */
        Color color;

        /**
         * ライン描画をする場合の幅を指定する
         */
        float lineWidth = 0;
    };

    /**
     * レンダリングコンテキストを生成させる
     * 必要に応じてカスタマイズクラスを返しても良い。
     *
     * コンテキストは使いまわされるので、必要に応じて1回だけ呼び出される。
     */
    virtual std::shared_ptr<RenderingContext> newRenderingContext(SpriteRenderer *sender) const = 0;

    /**
     * レンダリングを開始する
     */
    virtual void beginRendering(SpriteRenderer *sender) = 0;

    /**
     * 現在の状態に応じてレンダリングを行わせる
     *
     * @result レンダリングを実行できた場合はtrue
     */
    virtual bool requestRendering(SpriteRenderer *sender, const std::shared_ptr<RenderingContext> &context) = 0;


    /**
     * レンダリングを終了する
     */
    virtual void endRendering(SpriteRenderer *sender) = 0;


    virtual ~ISpriteRenderingCallback() = default;
};

}