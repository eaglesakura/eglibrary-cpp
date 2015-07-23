#pragma once

#include "es/eglibrary.hpp"

#include "es/math/Vector2.hpp"
#include "es/math/Rect.hpp"

namespace es {

/**
 * 2D座標系の変換を行わせる
 */
class IDisplayTransfer2D {
public:
    /**
     * スプライト座標を正規化デバイス座標に変換する
     */
    virtual Vector2f getSpritePositionToDevice(const int x2D, const int y2D) const = 0;

    virtual ~IDisplayTransfer2D() = default;
};

}