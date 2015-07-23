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
     */
    virtual RectI16 getSpriteArea() const = 0;

    virtual ~ITexture2D() = default;
};

}