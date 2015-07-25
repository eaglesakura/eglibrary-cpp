#include "es/graphics/PixelFormat.hpp"
#include "es/internal/protoground-internal.hpp"

namespace es {

void Pixel::copyRGB888Pixels(const uint8_t *src_rgb888, const PixelFormat_e dstFormat, uint8_t *dst, const uint _pixels) {
    if (src_rgb888 == dst) {
        return;
    }

    // 残ピクセル数
    uint pixels = _pixels;

    switch (dstFormat) {
        case PixelFormat_RGB565: {
            uint16_t *p = (uint16_t *) dst;
            while (pixels) {

                uint r = src_rgb888[0] & 0xff;
                uint g = src_rgb888[1] & 0xff;
                uint b = src_rgb888[2] & 0xff;

                (*p) = ((r >> 3) << 11) | ((g >> 2) << 5) | ((b >> 3));
                src_rgb888 += 3;
                ++p;
                --pixels;
            }
        }
            break;
        case PixelFormat_RGBA5551: {
            uint16_t *p = (uint16_t *) dst;
            while (pixels) {

                uint r = src_rgb888[0] & 0xff;
                uint g = src_rgb888[1] & 0xff;
                uint b = src_rgb888[2] & 0xff;
                (*p) = ((r >> 3) << 12) | ((g >> 3) << 6) | ((b >> 3) << 1) | 1;
                src_rgb888 += 3;
                ++p;
                --pixels;
            }
        }
            break;
        case PixelFormat_RGB888: {
            memcpy(dst, src_rgb888, pixels * 3);
        }
            break;
        case PixelFormat_RGBA8888: {
            while (pixels) {

                dst[0] = src_rgb888[0];
                dst[1] = src_rgb888[1];
                dst[2] = src_rgb888[2];
                dst[3] = 0xFF;

                src_rgb888 += 3;
                dst += 4;
                --pixels;
            }
        }
            break;
        default:
            assert(false); // not support!!
            break;
    }
}

unsigned int Pixel::toGLPixelDataType(const PixelFormat_e format) {
    constexpr const GLenum PIXEL_TYPES[] = {
            //
            GL_UNSIGNED_SHORT_5_6_5,// RGB565
            GL_UNSIGNED_SHORT_5_5_5_1, // RGBA5551
            GL_UNSIGNED_BYTE, // RGB888
            GL_UNSIGNED_BYTE, // RGBA8888
            GL_UNSIGNED_BYTE, // BGRA8888
            GL_HALF_FLOAT_OES, // LuminanceF16
            GL_HALF_FLOAT_OES, // RGBA F16
            GL_UNSIGNED_BYTE, // A8
            GL_UNSIGNED_SHORT, // Depth
            GL_UNSIGNED_BYTE, // Luminance8
            GL_UNSIGNED_BYTE, // R8
            //
    };
    return PIXEL_TYPES[format];
}

GLenum Pixel::toGLPixelFormat(const PixelFormat_e format) {
    static const GLenum PIXEL_FORMATS[] = {
            //
            GL_RGB,//   RGB565
            GL_RGBA,  //    RGBA5551
            GL_RGB,  // RGB888
            GL_RGBA, // RGBA8888
#ifdef GL_BGRA_EXT
            GL_BGRA_EXT, // BGRA8888
#else
            GL_RGBA, // RGBA8888
#endif
            GL_LUMINANCE, // L-16
            GL_RGBA, // RGBA F16
            GL_ALPHA, // A8
            GL_DEPTH_COMPONENT, // Depth
            GL_LUMINANCE, // L8
            GL_RED, // R8
            //
    };
    return PIXEL_FORMATS[format];
}

void Pixel::copyBGRA8888Pixels(const uint8_t *src_bgra8888, const PixelFormat_e dstFormat, uint8_t *dst, const uint _pixels) {
    if (src_bgra8888 == dst) {
        return;
    }

    // 残ピクセル数
    uint pixels = _pixels;

    const int pixel_size = 4;

    switch (dstFormat) {
        case PixelFormat_RGB565: {
            uint16_t *p = (uint16_t *) dst;
            while (pixels) {

                const uint r = src_bgra8888[2] & 0xff;
                const uint g = src_bgra8888[1] & 0xff;
                const uint b = src_bgra8888[0] & 0xff;

                (*p) = ((r >> 3) << 11) | ((g >> 2) << 5) | ((b >> 3));
                src_bgra8888 += pixel_size;
                ++p;
                --pixels;
            }
        }
            break;
        case PixelFormat_RGBA5551: {
            uint16_t *p = (uint16_t *) dst;
            while (pixels) {

                const uint r = src_bgra8888[2] & 0xff;
                const uint g = src_bgra8888[1] & 0xff;
                const uint b = src_bgra8888[0] & 0xff;
                const uint a = (src_bgra8888[3] & 0xff) > 0 ? 1 : 0;
                (*p) = ((r >> 3) << 11) | ((g >> 3) << 6) | ((b >> 3) << 1) | a;
                src_bgra8888 += pixel_size;
                ++p;
                --pixels;
            }
        }
            break;
        case PixelFormat_RGB888: {
            while (pixels) {

                dst[0] = src_bgra8888[2];
                dst[1] = src_bgra8888[1];
                dst[2] = src_bgra8888[0];

                src_bgra8888 += pixel_size;
                dst += 3;
                --pixels;
            }
        }
            break;
        case PixelFormat_RGBA8888: {
            while (pixels) {

                dst[0] = src_bgra8888[2];
                dst[1] = src_bgra8888[1];
                dst[2] = src_bgra8888[0];
                dst[3] = src_bgra8888[3];

                src_bgra8888 += pixel_size;
                dst += 4;
                --pixels;
            }
        }
            break;
        default:
            eslog("unknown dst format(%d)", dstFormat);
            assert(false); // not support!!
            break;
    }
}

void Pixel::copyRGBA8888Pixels(const uint8_t *src_rgba8888, const PixelFormat_e dstFormat, uint8_t *dst, const uint _pixels) {
    if (src_rgba8888 == dst) {
        return;
    }

    // 残ピクセル数
    uint pixels = _pixels;

    const int pixel_size = 4;

    switch (dstFormat) {
        case PixelFormat_RGB565: {
            uint16_t *p = (uint16_t *) dst;
            while (pixels) {

                const uint r = src_rgba8888[0] & 0xff;
                const uint g = src_rgba8888[1] & 0xff;
                const uint b = src_rgba8888[2] & 0xff;

                (*p) = ((r >> 3) << 11) | ((g >> 2) << 5) | ((b >> 3));
                src_rgba8888 += pixel_size;
                ++p;
                --pixels;
            }
        }
            break;
        case PixelFormat_RGBA5551: {
            uint16_t *p = (uint16_t *) dst;
            while (pixels) {

                const uint r = src_rgba8888[0] & 0xff;
                const uint g = src_rgba8888[1] & 0xff;
                const uint b = src_rgba8888[2] & 0xff;
                const uint a = (src_rgba8888[3] & 0xff) > 0 ? 1 : 0;
                (*p) = ((r >> 3) << 11) | ((g >> 3) << 6) | ((b >> 3) << 1) | a;
                src_rgba8888 += pixel_size;
                ++p;
                --pixels;
            }
        }
            break;
        case PixelFormat_RGB888: {
            while (pixels) {

                dst[0] = src_rgba8888[0];
                dst[1] = src_rgba8888[1];
                dst[2] = src_rgba8888[2];

                src_rgba8888 += pixel_size;
                dst += 3;
                --pixels;
            }
        }
            break;
        case PixelFormat_RGBA8888: {
            memcpy(dst, src_rgba8888, pixels * pixel_size);
        }
            break;

        default:
            eslog("unknown dst format(%d)", dstFormat);
            assert(false); // not support!!
            break;
    }
}
}