#pragma once

#include    "es/protoground.hpp"
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
     * 単色テクスチャ
     */
            PixelFormat_R8,

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
    PixelFormat_NativeRGBA = PixelFormat_BGRA8888,
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
    static void copyRGB888Pixels(const uint8_t *src_rgb888, const PixelFormat_e dstFormat, uint8_t *dst, const uint _pixels);

    /**
     * RGB888のポインタをdstへピクセル情報をコピーする。
     * 同じポインタの場合は、何も行わない。
     */
    static void copyRGBA8888Pixels(const uint8_t *src_rgba8888, const PixelFormat_e dstFormat, uint8_t *dst, const uint _pixels);


    /**
     * RGB888のポインタをdstへピクセル情報をコピーする。
     * 同じポインタの場合は、何も行わない。
     */
    static void copyBGRA8888Pixels(const uint8_t *src_bgra8888, const PixelFormat_e dstFormat, uint8_t *dst, const uint _pixels);

    /**
     * GLESで使用されるピクセルフォーマットへ変換する
     *
     * GL_RGBA等に変換される
     */
    static GLenum toGLPixelFormat(const PixelFormat_e format);

    /**
     * GLESで使用されるデータタイプへ変換する
     *
     * GL_UNSIGNED_BYTE等に変換される
     */
    static GLenum toGLPixelDataType(const PixelFormat_e format);
};

}
