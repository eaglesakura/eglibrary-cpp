#pragma once

#include "es/eglibrary.hpp"
#include "Texture.h"
#include "es/asset/image/IImageBufferListener.hpp"

namespace es {

class TextureLoadListener : public Object, public IImageBufferListener {
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

    std::shared_ptr<DeviceContext> device;
public:
    TextureLoadListener();

    virtual ~TextureLoadListener() = default;

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
     * NPOT変換を行わせる場合true
     */
    void setConvertNpotToPot(bool convertNpot);

    /**
     * 画像情報を読み込んだ
     */
    virtual void onImageInfoReceived(const ImageInfo *info);

    /**
     * 画像を指定行読み込んだ
     *
     * 引数lineは使いまわされる可能性があるため、内部的にテクスチャコピー等を行うこと。
     */
    virtual void onImageLineReceived(const ImageInfo *info, const unsafe_array<uint8_t> pixels, const uint height);
};

}



