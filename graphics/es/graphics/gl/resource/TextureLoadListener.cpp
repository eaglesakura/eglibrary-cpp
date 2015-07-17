#include "TextureLoadListener.h"

namespace es {

TextureLoadListener::TextureLoadListener() {

}

const std::shared_ptr<Texture> &TextureLoadListener::getTexture() const {
    return texture;
}

void TextureLoadListener::setTexture(const std::shared_ptr<Texture> &texture) {
    this->texture = texture;
}

void TextureLoadListener::setConvertNpotToPot(bool convertNpot) {
    TextureLoadListener::convertNpot = convertNpot;
}

void TextureLoadListener::onImageInfoReceived(const IImageBufferListener::ImageInfo *info) {
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

    texture->unbind(device);
}

void TextureLoadListener::onImageLineReceived(const IImageBufferListener::ImageInfo *info, const unsafe_array<uint8_t> pixels, const uint height) {
    texture->bind(device);
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
    texture->unbind(device);
}

void TextureLoadListener::setDevice(const std::shared_ptr<DeviceContext> &device) {
    this->device = device;
}

}