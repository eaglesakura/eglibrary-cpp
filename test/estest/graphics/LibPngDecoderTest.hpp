#pragma once

#include "estest/eglibrary-test.hpp"
#include "es/graphics/png/PngFileDecoder.h"

namespace es {
namespace test {


namespace internal {

sp<IImageBufferListener> newSimpleImageListener() {
    class PngImageListener : public IImageBufferListener {
        ImageInfo info;
        bool infoReceived = false;
        int readedLines = 0;
    public:

        PngImageListener() { }

        virtual ~PngImageListener() { }

        /**
         * 画像情報を読み込んだ
         */
        virtual void onImageInfoReceived(const ImageInfo *info) {
            ASSERT_NE(info, nullptr);
            ASSERT_TRUE(info->width);
            ASSERT_TRUE(info->height);

            this->info = *info;
            infoReceived = true;
        }

        /**
         * 画像を指定行読み込んだ
         *
         * 引数lineは使いまわされる可能性があるため、内部的にテクスチャコピー等を行うこと。
         */
        virtual void onImageLineReceived(const ImageInfo *info, const unsafe_array<uint8_t> pixels, const uint height) {
            ASSERT_TRUE(pixels.length > 0);
            ASSERT_TRUE(height);
            ASSERT_TRUE(infoReceived);
            readedLines += height;
            ASSERT_TRUE(readedLines <= (int) info->height);
        }
    };

    sp<IImageBufferListener> result(new PngImageListener());
    return result;
}

}

/**
 * 正方形PowerOfTwo PNG画像を読み込む
 */
TEST(LibPngDecoderTest, DecodeSquarePot_dstRGB8) {
    sp<IAsset> asset = AssetManager::load("png/square-pot.png");
    ASSERT_TRUE((bool) asset);

    PngFileDecoder decoder;
    decoder.setOnceReadHeight(25);
    decoder.setConvertPixelFormat(PixelFormat_RGB888);
    ASSERT_TRUE(decoder.load(asset, internal::newSimpleImageListener()));
}

/**
 * 正方形PowerOfTwo PNG画像を読み込む
 */
TEST(LibPngDecoderTest, DecodeSquarePot_dstRGBA8) {
    sp<IAsset> asset = AssetManager::load("png/square-pot.png");
    ASSERT_TRUE((bool) asset);

    PngFileDecoder decoder;
    decoder.setOnceReadHeight(25);
    decoder.setConvertPixelFormat(PixelFormat_RGBA8888);
    ASSERT_TRUE(decoder.load(asset, internal::newSimpleImageListener()));
}

}
}