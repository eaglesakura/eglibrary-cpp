#pragma once

#include "es/eglibrary.hpp"
#include "es/graphics/gl/resource/Texture.h"
#include "es/asset/image/IImageDecodeListener.hpp"

namespace es {

/**
 * デコード結果をテクスチャとして格納する
 */
class TextureDecodeListener : public Object, public IImageDecodeListener {
public:
    TextureDecodeListener();

    virtual ~TextureDecodeListener() = default;

    void setDevice(const std::shared_ptr<DeviceContext> &device);

    /**
     * 読み込まれたテクスチャを取得する
     */
    const std::shared_ptr<Texture> &getTexture() const;

    /**
     * 読み込み対象のテクスチャを指定する。
     */
    void setTexture(const std::shared_ptr<Texture> &texture);

    /**
     * テクスチャに焼きこむ時のXY座標を調整する
     */
    void setUploadOffset(const int x, const int y);


    /**
     * NPOT変換を行わせる場合true
     */
    void setConvertNpotToPot(bool convertNpot);

    /**
     * 画像情報を読み込んだ
     */
    virtual void onImageInfoDecoded(const ImageInfo *info);

    /**
     * 画像を指定行読み込んだ
     *
     * 引数lineは使いまわされる可能性があるため、内部的にテクスチャコピー等を行うこと。
     */
    virtual void onImageLineDecoded(const ImageInfo *info, const unsafe_array<uint8_t> pixels, const uint height);

    /**
     * 画像のデコードをキャンセルする場合はtrue
     */
    virtual bool isImageDecodeCancel();

    /**
     * デコードが完了した
     */
    virtual void onImageDecodeFinished(const ImageInfo *info, const ImageDecodeResult_e result);

private:
    /**
     * 読み込み対象のテクスチャ
     */
    std::shared_ptr<Texture> texture;

    /**
     * 書き込み先の座標Y
     */
    uint writePixelsY = 0;

    /**
     * NPOTをPOT変換をする場合true
     */
    bool convertNpot = false;

    int16_t offsetX = 0;
    int16_t offsetY = 0;

    std::shared_ptr<DeviceContext> device;
};

}



