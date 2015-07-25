#include    "es/Graphics.hpp"
#include "es/OpenGL.hpp"
#include    "SurfaceCaptor.h"

#include "es/internal/protoground-internal.hpp"


namespace es {

SurfaceCaptor::SurfaceCaptor(const RectI &area) {
    setArea(area);
}

SurfaceCaptor::SurfaceCaptor(int surfaceWidth, int surfaceHeight) {
    setArea(surfaceWidth, surfaceHeight);
}

/**
 * 指定フォーマットでキャプチャする
 */
ByteBuffer SurfaceCaptor::captureBuffer(PixelFormat_e format) {
    ByteBuffer buffer = Pixel::createPixelBuffer(format, area.getAreaSize());
    capture(format, buffer.get());
    return buffer;
}

/**
 * 画像のキャプチャを行う
 */
void SurfaceCaptor::capture(MRawImage dst) {
    capture(dst->getPixelFormat(), dst->getPixels());
}

/**
 * キャプチャを行う
 */
void SurfaceCaptor::capture(PixelFormat_e format, void *dst) {
    // Alignment指定
    switch (format) {
        case PixelFormat_A8:
        case PixelFormat_RGB565:
        case PixelFormat_RGB888:
            glPixelStorei(GL_PACK_ALIGNMENT, 1);
            break;
        case PixelFormat_BGRA8888:
        case PixelFormat_RGBA8888:
            glPixelStorei(GL_PACK_ALIGNMENT, 4);
            break;
        default:
            eslog("not support format(%d)", format);
            assert(false);
    }

    glReadPixels(area.left, area.top, area.width(), area.height(), Pixel::toGLPixelFormat(format), Pixel::toGLPixelDataType(format), dst);
    assert_gl();
    glFinish();
}

}
