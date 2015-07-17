#include "PngFileDecoder.h"
#include <png.h>
#include "es/internal/eglibrary-internal.hpp"

namespace es {

PngFileDecoder::PngFileDecoder() {

}

PngFileDecoder::~PngFileDecoder() {

}

namespace internal {
struct ImageBufferReader {
    unsafe_array<uint8_t> preLoad;
    shared_ptr<IAsset> asset;

    /**
     * バッファを読み出す
     */
    bool loadBuffer(uint8_t *result, uint length) {
        if (preLoad) {
            // 読み込み可能な長さ分だけ読み込む
            int read = std::min(preLoad.length, (int) length);
            memcpy(result, preLoad.ptr, read);

            // 読み込んだ分だけオフセットして、残りの長さを縮める
            result += read;
            preLoad += read;
            length -= read;
        }

        // アセットから必要な長さを読み出す
        if (length) {
            unsafe_array<uint8_t> buffer = asset->read(length);
            if (buffer.length < length) {
                // 容量が足りない！
                memcpy(result, buffer.ptr, buffer.length);
                return false;
            } else {
                assert(buffer.length == length);
                // 十分な容量を読み込めた
                memcpy(result, buffer.ptr, length);
            }
        }

        return true;
    }
};
}

static void pngReadBuffer(png_structp png, png_bytep result, png_size_t readSize) {
//    eslog("PNG read request(%d bytes)", readSize);
    internal::ImageBufferReader *reader = (internal::ImageBufferReader *) png_get_io_ptr(png);
    if (!reader->loadBuffer((uint8_t *) result, readSize)) {
        eslog("PNG BufferOver!!");
    }
}

bool PngFileDecoder::load(std::shared_ptr<IAsset> asset, selection_ptr<IImageBufferListener> listener) {
    IImageBufferListener::ImageInfo info;
    info.dstPixelFormat = pixelConvert;

    internal::ImageBufferReader reader;
    reader.asset = asset;
    reader.preLoad = unsafe_array<uint8_t>(readedBuffer.get(), readedBuffer.length());

#if 0
    // シグニチャチェック
    {
        vector<uint8_t> sign(8);
        assert(sign.size() == 8);
        assert(sign.capacity() == 8);

        if (!reader.loadBuffer(util::asPointer(sign), sign.size())) {
            eslog("buffer read failed");
            return false;
        }

        if (png_sig_cmp(util::asPointer(sign), 0, sign.size())) {
            eslog("png_sig_cmp failed");
            return false;
        }
    }
#endif

    // PNG初期化
    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    assert(png);
    png_infop pngInfo = png_create_info_struct(png);
    assert(pngInfo);

    if (setjmp(png_jmpbuf(png))) {
        eslog("setjmp failed");
        return false;
    }

    png_set_read_fn(png, (void *) (&reader), pngReadBuffer);

    // info
    png_read_info(png, pngInfo);

    // 読み込みに必要なテンポラリを生成する
    info.width = png_get_image_width(png, pngInfo);
    info.height = png_get_image_height(png, pngInfo);
    info.format = IImageBufferListener::ImageFormat_PNG;
    int depth = png_get_bit_depth(png, pngInfo);
    int rowBytes = png_get_rowbytes(png, pngInfo);
    int perPixelBytes = rowBytes / info.width;
    {

        if (!info.width || !info.height) {
            return false;
        }

        eslog("PNG [%d x %d] %d bits = %d bytes/pixel [row = %d bytes]", info.width, info.height, depth, perPixelBytes, rowBytes);


        if (depth == 8) {
            if (perPixelBytes == 3) {
                info.srcPixelFormat = PixelFormat_RGB888;
            } else if (perPixelBytes == 4) {
                info.srcPixelFormat = PixelFormat_RGBA8888;
            } else {
                eslog("Not Support PNG depth bits...");
                return false;
            }
        } else {
            eslog("Not Support PNG depth bits...");
            return false;
        }

        // インフォメーションをコールバックする
        listener->onImageInfoReceived(&info);
    }


    // 一度に読み込む行数を確定する
    int onceReadLines = std::min((int) info.height, (int) this->onceReadLines);
    if (onceReadLines == 0) {
        onceReadLines = info.height;
    }

    vector<uint8_t> readCacheBuffer(rowBytes * onceReadLines);
    ByteBuffer convertBuffer;

    if (info.srcPixelFormat != this->pixelConvert) {
        // 変換先の画素を生成する
        convertBuffer = Pixel::createPixelBuffer(pixelConvert, rowBytes * onceReadLines);
    }

    vector<void *> rowHeaders(onceReadLines);

    for (int i = 0; i < onceReadLines; ++i) {
        rowHeaders[i] = (void *) util::asPointer(readCacheBuffer, rowBytes * i);
    }

    int lines = info.height;
    while (lines) {
        uint32_t reading = std::min((uint32_t) lines, (uint32_t) onceReadLines);
        eslog("PNG read[%d] Lines", reading);
        png_read_rows(png, NULL, (png_bytepp) util::asPointer(rowHeaders), reading);
        lines -= reading;

        // バッファをリスナに伝える
        if (convertBuffer.empty()) {
            listener->onImageLineReceived(&info, unsafe_array<uint8_t>(util::asPointer(readCacheBuffer), reading * rowBytes), reading);
        } else {
            eslog("PNG Convert[%d] -> [%d]", info.srcPixelFormat, pixelConvert);
            if (info.srcPixelFormat == PixelFormat_RGB888) {
                Pixel::copyRGB888Pixels(util::asPointer(readCacheBuffer), pixelConvert, convertBuffer.get(), onceReadLines * info.width);
            } else if (info.srcPixelFormat == PixelFormat_RGBA8888) {
                Pixel::copyRGBA8888Pixels(util::asPointer(readCacheBuffer), pixelConvert, convertBuffer.get(), onceReadLines * info.width);
            } else {
                eslog("PNG Convert not support...");
                return false;
            }
            listener->onImageLineReceived(&info, unsafe_array<uint8_t>(util::asPointer(readCacheBuffer), reading * rowBytes), reading);
        }
    }

//    png_read_end(png, pngInfo);
    png_destroy_read_struct(&png, &pngInfo, nullptr);
    assert(!png);
    assert(!pngInfo);
    return true;
}

void PngFileDecoder::setReadedBuffer(const unsafe_array<uint8_t> &buffer) {
    this->readedBuffer = Buffer::clone(buffer);
}

void PngFileDecoder::setOnceReadHeight(const uint heightPixels) {
    this->onceReadLines = heightPixels;
}

void PngFileDecoder::setConvertPixelFormat(const PixelFormat_e format) {
    this->pixelConvert = format;
}

}