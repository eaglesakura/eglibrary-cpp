#include "MmdBufferDataLoader.h"

namespace es {

/**
 * 文字数を指定して識別子を読み込む
 */
std::string MmdBufferDataLoader::loadTextBuffer() {
    uint32_t len;
    loadBuffer(&len, 4);
    if (len == 0) {
        return std::string();
    }

    // バッファを変換する

    // 文字列を変換する
    if (encoder) {
        safe_array<uint8_t> buffer(len + 1);
        loadBuffer(buffer.ptr, (uint) len);
        buffer.ptr[len] = 0;

        return encoder->encode(buffer.ptr);
    } else {
        safe_array<uint8_t> buffer(len + 1);
        loadBuffer(buffer.ptr, (uint) len);
        buffer.ptr[len] = 0;

        return std::string((char *) buffer.ptr);
    }

}

void MmdBufferDataLoader::setTextEncodeType(uint8_t type) {
    this->encodeType = (TextEncodeType) type;
    if (encodeType == UTF16) {
        encoder.reset(new StringEncoder(CHARSET_UTF8, CHARSET_UTF16LE));
    }
}

std::string MmdBufferDataLoader::loadEncodedString(int length, StringEncoder *encoder) {

    safe_array<uint8_t> buffer(length);
    buffer.zeromemory();
    loadBuffer(buffer.ptr, buffer.length);

    if (encoder) {
        return encoder->encode(buffer.ptr);
    } else {
        return std::string((char *) buffer.ptr);
    }
}
}