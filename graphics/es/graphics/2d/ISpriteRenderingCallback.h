#pragma once

#include "es/eglibrary.hpp"
#include "es/graphics/2d/ITexture2D.h"
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

    struct RenderingInstance {
        /**
         * ブレンドしたい色が指定される
         */
        Color color;


        /**
         * 回転角が設定される
         */
        float rotateRadian = 0;
    };

    struct RenderingLineInstance : public RenderingInstance {
        /**
         * ライン描画をする場合の幅を指定する
         */
        float lineWidth = 0;

        /**
         * レンダリング先のディスプレイ座標（線描画）
         */
        struct {
            Vector2f start;
            Vector2f end;
        } dstLine;
    };

    struct RenderingQuadInstance : public RenderingInstance {
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
         * レンダリング先のディスプレイ座標（四角形描画）
         * ピクセルではなく、正規化座標が入力される
         */
        RectF dstQuad;
    };

    struct RenderingState {
        /**
         * レンダリングモードを指定する
         */
        RenderingMode_e mode = RenderingMode_QuadFill;


        /**
         * レンダリングサーフェイスのアスペクト比
         */
        float surfaceAspect = 1;
    };

    /**
     * レンダリングを開始する
     */
    virtual void beginRendering(SpriteRenderer *sender) = 0;

    /**
     * 現在の状態に応じてレンダリングを行わせる
     *
     * 同じオブジェクトを何度も一括でレンダリングする（パーティクル等）場合のオーバーヘッドを少なくするため、バッチレンダリングを行えるようにする。
     * レンダリングインスタンスはmodeに合わせてRenderingQuadInstance*等にキャストする必要がある
     *
     * @result レンダリングしたインスタンス数を返す
     */
    virtual int requestRendering(SpriteRenderer *sender, const RenderingState *state, const uint numInstances, RenderingInstance *instanceArray) = 0;

    /**
     * レンダリングを終了する
     */
    virtual void endRendering(SpriteRenderer *sender) = 0;


    virtual ~ISpriteRenderingCallback() = default;
};

}