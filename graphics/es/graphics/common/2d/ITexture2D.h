#pragma once

#include "es/eglibrary.hpp"
#include "es/math/Vector2.hpp"
#include "es/math/Rect.hpp"

namespace es {

/**
 * 抽象化されたテクスチャ
 *
 */
class ITexture2D {
public:
    /**
     * Spriteの画像として利用可能なエリアを取得する
     *
     * @param size VRAM上のテクスチャサイズ
     * @param existArea 有効なテクスチャのエリアを取得する
     */
    virtual void getImageArea(Vector2i16 *size, RectI16 *existArea) = 0;

    virtual ~ITexture2D() = default;
};

}