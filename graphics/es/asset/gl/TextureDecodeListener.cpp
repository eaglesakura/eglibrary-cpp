#include "TextureDecodeListener.h"

namespace es {

TextureDecodeListener::TextureDecodeListener() {

}

const std::shared_ptr<Texture> &TextureDecodeListener::getTexture() const {
    return texture;
}

void TextureDecodeListener::setTexture(const std::shared_ptr<Texture> &texture) {
    this->texture = texture;
}

void TextureDecodeListener::setConvertNpotToPot(bool convertNpot) {
    TextureDecodeListener::convertNpot = convertNpot;
}

void TextureDecodeListener::onImageInfoDecoded(const IImageDecodeListener::ImageInfo *info) {
    this->writePixelsY = 0;
    if (!texture) {
        texture.reset(new Texture());
    }

    assert(device);
    assert(texture);


    int allocWidth = info->width;
    int allocHeight = info->height;
    if (convertNpot) {
        allocWidth = toPowerOfTwo(allocWidth);
        allocHeight = toPowerOfTwo(allocHeight);
    }
    texture->bind(device);

    // デフォルトを修正
    texture->setWrapMode(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
    texture->setFilter(GL_NEAREST, GL_NEAREST);

    // 転送パッキングを修正
    if (Pixel::getPixelBytes(info->dstPixelFormat) == 4) {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    } else {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    }


    // VRAM領域の確保
    texture->allocPixelMemory(info->dstPixelFormat, 0, allocWidth, allocHeight);
    texture->setImageSize(info->width, info->height);
    texture->onAllocated();
}

void TextureDecodeListener::onImageLineDecoded(const IImageDecodeListener::ImageInfo *info, const unsafe_array<uint8_t> pixels, const uint height) {
    // 部分転送 for 2D
    glTexSubImage2D(
            GL_TEXTURE_2D,
            0,
            0, writePixelsY,
            info->width, height,
            Pixel::toGLPixelFormat(info->dstPixelFormat),
            Pixel::toGLPixelDataType(info->dstPixelFormat),
            (void *) pixels.ptr
    );
    writePixelsY += height;
}

void TextureDecodeListener::setDevice(const std::shared_ptr<DeviceContext> &device) {
    this->device = device;
}

bool TextureDecodeListener::isImageDecodeCancel() {
    return false;
}

void TextureDecodeListener::onImageDecodeFinished(const IImageDecodeListener::ImageInfo *info, const IImageDecodeListener::ImageDecodeResult_e result) {
    texture->unbind(device);
}

}