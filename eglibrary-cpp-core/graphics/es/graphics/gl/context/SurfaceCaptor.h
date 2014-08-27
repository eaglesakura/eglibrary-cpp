#ifndef es_graphics_gl_SURFACECAPTOR_H_
#define es_graphics_gl_SURFACECAPTOR_H_

#include    "es/math/Rect.hpp"
#include    "es/graphics/PixelFormat.hpp"
#include    "es/graphics/RawImage.h"

namespace es {

/**
 * 現在のレンダリング対象をキャプチャする
 *
 * キャプチャするareaは左下原点のGL-2D座標系であることに注意する
 */
class SurfaceCaptor {
    /**
     * キャプチャするエリア
     */
    RectI area;
public:
    SurfaceCaptor(const RectI &area);
    SurfaceCaptor(int surfaceWidth, int surfaceHeight);

    void setArea(const RectI &area) {
        this->area = area;
    }

    void setArea(int surfaceWidth, int surfaceHeight) {
        this->area.setXYWH(0, 0, surfaceWidth, surfaceHeight);
    }


    /**
     * 指定フォーマットでキャプチャする
     */
    ByteBuffer captureBuffer(PixelFormat_e format);

    /**
     * 画像のキャプチャを行う
     */
    void capture(MRawImage dst);

    /**
     * キャプチャを行う
     */
    void capture(PixelFormat_e format, void* dst);
};

}

#endif /* SURFACECAPTOR_H_ */
