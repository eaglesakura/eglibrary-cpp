#ifndef es_graphics_gl_THREADDEVICE_H_
#define es_graphics_gl_THREADDEVICE_H_

#include    "es/Graphics.hpp"
#include    "es/math/Vector2.hpp"
#include    "es/graphics/gl/context/RenderState.h"

namespace es {

/**
 * Threadに関連付けられたデバイスを管理する
 */
class ThreadDevice: public Object {
    /**
     * 大本のサーフェイスのデフォルトサイズ
     */
    Vector2i16 surfaceSize;

    /**
     * レンダリング用ステート
     */
    MRenderState renderState;
protected:
    ThreadDevice();
public:
    virtual ~ThreadDevice();

    /**
     * サーフェイズのサイズを設定する
     */
    void setSurfaceSize(int x, int y) {
        assert(x >= 0 && y >= 0);
        surfaceSize.set((int16_t) x, (int16_t) y);
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
    static std_shared_ptr<ThreadDevice> current();

    /**
     * 現在のスレッドで使用しなくなった
     */
    static void unuseThisThread();
};

/**
 * Managed
 */
typedef std_shared_ptr<ThreadDevice> MThreadDevice;

}

#endif /* THREADDEVICE_H_ */
