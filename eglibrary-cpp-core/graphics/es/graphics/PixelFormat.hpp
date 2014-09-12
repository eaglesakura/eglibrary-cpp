#ifndef es_graphics_PIXELFORMAT_HPP_
#define es_graphics_PIXELFORMAT_HPP_

#include    "es/eglibrary.hpp"
#include    "es/Graphics.hpp"
#include    "es/memory/Buffer.hpp"

namespace es {

/**
 * ピクセルフォーマットを指定する
 */
enum PixelFormat_e {
    /**
     * 2byte
     * GL_UNSIGNED_SHORT_5_6_5
     */
    PixelFormat_RGB565,

    /**
     * 2byte
     * GL_UNSIGNED_SHORT_5_5_5_1
     */
    PixelFormat_RGBA5551,

    /**
     * 3byte
     * GL_UNSIGNED_BYTE
     */
    PixelFormat_RGB888,

    /**
     * 4byte
     * GL_UNSIGNED_BYTE
     */
    PixelFormat_RGBA8888,

    /**
     * 4byte
     * GL_UNSIGNED_BYTE
     */
    PixelFormat_BGRA8888,

    /**
     * 2byte
     * GL_HALF_FLOAT
     */
    PixelFormat_LuminanceF16,

    /**
     * 8byte
     * GL_HALF_FLOAT
     */
    PixelFormat_RGBA_F16,

    /**
     * 1byte
     * GL_ALPHA
     */
    PixelFormat_A8,

    /**
     * 2byte
     * Depth
     */
    PixelFormat_Depth,

    /**
     * 輝度限定テクスチャ
     */
    PixelFormat_Luminance8,

    /**
     * RGBA各要素を含み、プラットフォームに最適化した状態でロードする
     * 各1byteは保証するが、RGBA並び順は保証しない。
     *
     * Android::高速に読み込めるため、その順で格納される。
     * 4byte
     * GL_BGRA_EXT
     * GL_UNSIGNED_BYTE
     */
#ifdef  BUILD_Android
    PixelFormat_NativeRGBA = PixelFormat_RGBA8888,
#else
PixelFormat_NativeRGBA = PixelFormat_RGBA8888,
#endif
};

class Pixel {
public:

/**
 * フォーマットごとの1ピクセルサイズを取得する
 */
static inline const uint getPixelBytes(const PixelFormat_e fmt) {
    constexpr const uint size[] = {
    //
            2,// RGB565
            2, // RGBA5551
            3, // RGB888
            4, // RGBA8888
            4, // BGRA8888
            2, // LuminanceF16
            8, // RGBA_F16
            1, // A8
            1, // Luminance8
            };

    return size[fmt];
}

/**
 * 指定したピクセルフォーマットで規定のピクセル数分のバッファを作成する
 */
static ByteBuffer createPixelBuffer(const PixelFormat_e fmt, const uint pixels) {
    uint bytes = getPixelBytes(fmt);
    return Buffer::create(bytes * pixels);
}

/**
 * RGB888のポインタをdstへピクセル情報をコピーする。
 * 同じポインタの場合は、何も行わない。
 */
inline
static void copyRGB888Pixels(const uint8_t *src_rgb888, const PixelFormat_e dstFormat, uint8_t *dst, const uint _pixels) {
    if (src_rgb888 == dst) {
        return;
    }

    // 残ピクセル数
    uint pixels = _pixels;

    switch (dstFormat) {
    case PixelFormat_RGB565: {
        uint16_t *p = (uint16_t*) dst;
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
        uint16_t *p = (uint16_t*) dst;
        while (pixels) {

            uint r = src_rgb888[0] & 0xff;
            uint g = src_rgb888[1] & 0xff;
            uint b = src_rgb888[2] & 0xff;
            (*p) = ((r >> 3) << 11) | ((g >> 3) << 5) | ((b >> 3) << 1) | 1;
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

/**
 * RGB888のポインタをdstへピクセル情報をコピーする。
 * 同じポインタの場合は、何も行わない。
 */
inline
static void copyRGBA8888Pixels(const uint8_t *src_rgba8888, const PixelFormat_e dstFormat, uint8_t *dst, const uint _pixels) {
    if (src_rgba8888 == dst) {
        return;
    }

    // 残ピクセル数
    uint pixels = _pixels;

    const int pixel_size = 4;

    switch (dstFormat) {
    case PixelFormat_RGB565: {
        uint16_t *p = (uint16_t*) dst;
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
        uint16_t *p = (uint16_t*) dst;
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

    /**
     * RGB888のポインタをdstへピクセル情報をコピーする。
     * 同じポインタの場合は、何も行わない。
     */
}

inline
static void copyBGRA8888Pixels(const uint8_t *src_bgra8888, const PixelFormat_e dstFormat, uint8_t *dst, const uint _pixels) {
    if (src_bgra8888 == dst) {
        return;
    }

    // 残ピクセル数
    uint pixels = _pixels;

    const int pixel_size = 4;

    switch (dstFormat) {
    case PixelFormat_RGB565: {
        uint16_t *p = (uint16_t*) dst;
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
        uint16_t *p = (uint16_t*) dst;
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

/**
 * GLESで使用されるピクセルフォーマットへ変換する
 *
 * GL_RGBA等に変換される
 */
static inline GLenum toGLPixelFormat(const PixelFormat_e format) {
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
            //
            };
    return PIXEL_FORMATS[format];
}

/**
 * GLESで使用されるデータタイプへ変換する
 *
 * GL_UNSIGNED_BYTE等に変換される
 */
static inline GLenum toGLPixelDataType(const PixelFormat_e format) {
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
            //
            };
    return PIXEL_TYPES[format];
}

};
}

#endif /* PIXELFORMAT_HPP_ */
