#pragma once

namespace es {

/**
 * フォーマットに依存しない画素情報の受け取りを行う
 */
/**
 * 画像読み込み時にコールバックを受け取る
 */
class IImageBufferListener {
public:
    enum ImageFormat_e {
        ImageFormat_PNG,
    };

    /**
     * 画像の基本情報
     */
    struct ImageInfo {
        uint16_t width;
        uint16_t height;

        ImageFormat_e format;

        /**
         * 元画像ピクセルフォーマット
         */
        PixelFormat_e srcPixelFormat;

        /**
         * 変換されたピクセルフォーマット
         */
        PixelFormat_e dstPixelFormat;
    };

    /**
     * 画像情報を読み込んだ
     *
     * infoの所有権はDecoderにあるため、ポインタを保存してはならない。
     */
    virtual void onImageInfoReceived(const ImageInfo *info) = 0;

    /**
     * 画像を指定行読み込んだ
     *
     * 引数lineは使いまわされる可能性があるため、内部的にテクスチャコピー等を行うこと。
     */
    virtual void onImageLineReceived(const ImageInfo *info, const unsafe_array<uint8_t> pixels, const uint height) = 0;
};


}