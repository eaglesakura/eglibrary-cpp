#pragma once

#include    "es/Graphics.hpp"
#include    "es/math/Vector2.hpp"
#include    "es/graphics/gl/context/RenderState.h"
#include    "es/graphics/gl/context/ShaderState.h"

namespace es {

/**
 * Threadに関連付けられたデバイスを管理する
 */
class DeviceContext : public Object {
public:
    virtual ~DeviceContext();

    /**
     * サーフェイズのサイズを設定する
     */
    void setSurfaceSize(int x, int y) {
        assert(x >= 0 && y >= 0);
        surfaceSize.set((int16_t) x, (int16_t) y);
    }

    /**
     * 標準的な左上2D座標系を基本としてViewportを指定する
     */
    void viewport2D(int x, int y, int w, int h) {
        y = surfaceSize.y - h + y; // 必要なサイズだけ上げ底にする
        getRenderState()->viewport(x, y, w, h);
    }

    const Vector2i16 &getSurfaceSize() const {
        return surfaceSize;
    }

    MRenderState getRenderState() const {
        return renderState;
    }

    MShaderState getShaderState() const {
        return shaderState;
    }

    /**
     * 2Dレンダリングのデフォルトステートを取得する
     */
    glstates createDefaultState2D() const;

    /**
     * 3Dレンダリングのデフォルトステートを取得する
     */
    glstates createDefaultState3D() const;

    /**
     * 現在のThreadに関連付けられたデバイスを取得する
     */
    static ::std::shared_ptr<DeviceContext> current();

    /**
     * 現在のスレッドで使用しなくなった
     */
    static void unuseThisThread();

    /**
     * 有効なコンテキスト数を取得する
     */
    static uint getContextNum();

protected:
    /**
     * 大本のサーフェイスのデフォルトサイズ
     */
    Vector2i16 surfaceSize;

    /**
     * レンダリング用ステート
     */
    MRenderState renderState;

    /**
     * シェーダー用ステート
     */
    MShaderState shaderState;

    DeviceContext();
};

/**
 * Managed
 */
typedef ::std::shared_ptr<DeviceContext> MDeviceContext;

}
