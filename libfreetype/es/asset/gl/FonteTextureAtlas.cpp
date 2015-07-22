#include "FonteTextureAtlas.h"
#include "es/asset/FontCharactor.h"

namespace es {

namespace internal {

class BasicTextureAllocator : public FontTextureAtlas::TextureAllocator {
    /**
     * 生成するテクスチャのサイズ
     */
    uint size;

public:
    BasicTextureAllocator(uint newSize) : size(newSize) {
        assert(isPowerOfTwo(newSize));
        assert(newSize >= 2);
    }

    /**
     * 新規にテクスチャを生成させる
     *
     * テクスチャは最低でもフォントの大きさ以上の解像度を持つ必要がある。
     */
    virtual std::shared_ptr<Texture> newTexture(sp<DeviceContext> context) {
        sp<Texture> result(new Texture());
        result->bind(context);
        result->setWrapMode(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
        result->setFilter(GL_NEAREST, GL_NEAREST);
        result->allocPixelMemory(PixelFormat_R8, 0, size, size);

        const uint BUFFER_DIV = 8;
        const uint BUFFER_SIZE = size / BUFFER_DIV;
        std::vector<uint8_t> cache((BUFFER_SIZE) * (BUFFER_SIZE));
        util::zeromemory(&cache);
        for (int i = 0; i < BUFFER_DIV; ++i) {
            for (int k = 0; k < BUFFER_DIV; ++k) {
                // 空のバッファをアップロードしてテクスチャを空白にする
                upload(result,
                       BUFFER_SIZE * k, BUFFER_SIZE * i,
                       BUFFER_SIZE, BUFFER_SIZE,
                       unsafe_array<uint8_t>(util::asPointer(cache), cache.size())
                );
            }
        }
        result->unbind(context);
        return result;
    }

    /**
     * 画素情報のアップロードを行わせる
     */
    virtual void upload(std::shared_ptr<Texture> texture,
                        const uint offsetX, const uint offsetY,
                        const uint width, const uint height,
                        const unsafe_array<uint8_t> &buffer) {
        glTexSubImage2D(
                GL_TEXTURE_2D,
                0,
                offsetX, offsetY,
                width, height,
                Pixel::toGLPixelFormat(PixelFormat_R8),
                Pixel::toGLPixelDataType(PixelFormat_R8),
                (void *) buffer.ptr
        );
        assert_gl();
    }

};

}

FontTextureAtlas::FontTextureAtlas(const std::shared_ptr<FontFace> newFont) : font(newFont) {
    assert(font);

    allocator.reset(new internal::BasicTextureAllocator(1024));
}

uint FontTextureAtlas::bake(std::shared_ptr<DeviceContext> context, const std::wstring text) {
    class TempListener : public IImageDecodeListener {
    public:
        ImageInfo info;
        unsafe_array<uint8_t> pixels;
        ImageDecodeResult_e result = ImageDecodeResult_UnknownError;

        virtual void onImageInfoDecoded(const ImageInfo *info) {
            this->info = *info;
        }

        /**
         * 画像を指定行読み込んだ
         *
         * 引数lineは使いまわされる可能性があるため、内部的にテクスチャコピー等を行うこと。
         */
        virtual void onImageLineDecoded(const ImageInfo *info, const unsafe_array<uint8_t> pixels, const uint height) {
            this->pixels = pixels;
        }

        /**
         * 画像のデコードをキャンセルする場合はtrue
         */
        virtual bool isImageDecodeCancel() {
            return false;
        }

        /**
         * デコードが完了した
         */
        virtual void onImageDecodeFinished(const ImageInfo *info, const ImageDecodeResult_e result) {
            this->result = result;
        }
    } tempListener;

    uint result = 0;
    for (const wchar_t code : text) {
        if (atlasMap.find(code) != atlasMap.end()) {
            // 既にatlasを保持している
            continue;
        }

        // 新規にAtlasを生成する
        if (textures.empty()) {
            auto texture = allocator->newTexture(context);
            textures.push_back(texture);
        }
        auto texture = textures[textures.size() - 1];
        auto charactor = font->rendering(code, &tempListener);
        if (charactor && tempListener.result == IImageDecodeListener::ImageDecodeResult_Success) {
            // レンダリングに成功したので、テクスチャに焼きこむ
            Vector2i16 bitmapSize = charactor->getBitmapSize();
            if ((current.x + bitmapSize.x + 1) > texture->getWidth()) {
                // この行をはみ出す場合は改行する
                current.y += (current.lineHeight + 1);
                current.x = 0;
                current.lineHeight = 0;
            }
            if ((current.y + bitmapSize.y) > texture->getHeight()) {
                // テクスチャからはみ出る場合はテクスチャも生成する
                textures.push_back(allocator->newTexture(context));
                texture = textures[textures.size() - 1];
                assert(texture);
                current.y = 0;
            }

            sp<FontArea> area(new FontArea());
            area->area.left = current.x;
            area->area.top = current.y;
            area->area.right = area->area.left + tempListener.info.width;
            area->area.bottom = area->area.top + tempListener.info.height;
            area->charactor = charactor;
            area->index = textures.size() - 1;

            // 焼きこみを行う
            {
                texture->bind(context);
                allocator->upload(texture,
                                  area->area.left, area->area.top,
                                  area->area.width(), area->area.height(),
                                  tempListener.pixels
                );
                texture->unbind(context);
            }

            // 次のテクスチャ座標を設定
            current.x += (area->area.width() + 1);
            current.lineHeight = std::max<int16_t>(area->area.height() + 1, current.lineHeight);

            // アトラス情報を保存
            atlasMap.insert(std::pair<wchar_t, sp<FontArea> >(code, area));
        }
        ++result;
    }

    return result;
}

const std::vector<std::shared_ptr<es::Texture> > &FontTextureAtlas::getTextures() const {
    return textures;
}

void FontTextureAtlas::setAllocator(std::shared_ptr<TextureAllocator> allocator) {

}

const std::shared_ptr<FontTextureAtlas::FontArea> FontTextureAtlas::pick(const wchar_t charactor) const {
    return find(atlasMap, charactor);
}

uint16_t FontTextureAtlas::FontArea::getIndex() const {
    return index;
}

std::shared_ptr<FontCharactor> FontTextureAtlas::FontArea::getCharactor() const {
    return charactor;
}

const RectI16 &FontTextureAtlas::FontArea::getArea() const {
    return area;
}
}
