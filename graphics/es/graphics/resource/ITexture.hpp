#pragma once

#include "es/eglibrary.hpp"
#include "es/math/Vector2.hpp"
#include "es/math/Rect.hpp"

namespace es {

/**
 * 抽象化されたテクスチャ
 *
 */
class ITexture {
public:
    enum Type_e {
        /**
         * 一次元テクスチャ
         */
                Type_1D,

        /**
         * 通常の二次元テクスチャ
         */
                Type_2D,

        /**
         * キューブマップ用テクスチャ
         */
                Type_Cube,

        /**
         * 外部リソース画像(SurfaceTexture等)
         * for Android
         */
                Type_AndroidExternalImage,

        /**
         * 不明なタイプ
         */
                Type_Unknown,
    };

    /**
     * Spriteの画像として利用可能なエリアを取得する
     *
     * @param size VRAM上のテクスチャサイズ
     * @param existArea 有効なテクスチャのエリアを取得する
     */
    virtual void getImageArea(Vector2i16 *size, RectI16 *existArea) = 0;

    /**
     * テクスチャの種類を取得する
     */
    virtual Type_e geType() const = 0;

    virtual ~ITexture() = default;
};

}