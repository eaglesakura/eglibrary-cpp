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
    struct ConvertOption {

    };

    /**
     * スプライト座標を正規化デバイス座標に変換する
     *
     * 引数XYは長さベクトルもしくは座標となる。
     */
    virtual Vector2f getSpriteToNormalizedDevice(const int x2D, const int y2D, const ConvertOption *option = nullptr) = 0;

    virtual ~IDisplayTransfer2D() = default;
};

}