//
// Created by @eaglesakura on 15/05/08.
//

#ifndef ASSEMBLE_MMDBUFFERDATALOADER_HPP
#define ASSEMBLE_MMDBUFFERDATALOADER_HPP

#include <es/graphics/Color.hpp>
#include <es/math/Vector3.hpp>
#include    "es/memory/SafeArray.hpp"

namespace es {

/**
 * MMD形式のデータをバッファから読み出す
 */
class MmdBufferDataLoader : public Object {
    unsafe_array<uint8_t> buffer;


    enum TextEncodeType {
        UTF16,
        UTF8,
        Unknown,
    } encodeType = Unknown;
public:

    MmdBufferDataLoader(const unsafe_array<uint8_t> &buffer) {
        this->buffer = buffer;
    }

    virtual ~MmdBufferDataLoader() { }

    virtual void setTextEncodeType(uint8_t type) {
        this->encodeType = (TextEncodeType) type;
    }

    /**
     * 文字数を指定して識別子を読み込む
     */
    virtual std::string loadFileMagic(uint chars) {
        safe_array<uint8_t> buffer(chars + 1);
        loadBuffer(buffer.ptr, chars);
        buffer.ptr[chars] = 0;
        return std::string((char *) buffer.ptr);
    }

    /**
     * テキストバッファを読み出す
     */
    virtual std::string loadTextBuffer();

    /**
     * バッファを読み込む
     */
    virtual void loadBuffer(void *result, uint bytes) {
        assert(buffer.length >= bytes);
        memcpy(result, buffer.ptr, bytes);
        buffer += (int) bytes;
    }

    virtual uint8_t loadByte() {
        auto result = *buffer.ptr;
        ++buffer;
        return result;
    }

    virtual int32_t loadIntN(uint bytes) {
        switch (bytes) {
            case 1:
                return loadInt8();
            case 2:
                return loadInt16();
            case 4:
                return loadInt32();
            default:
                assert(false);
                return 0;
        }
    }

    virtual int8_t loadInt8() {
        int8_t result;
        loadBuffer(&result, sizeof(result));
        return result;
    }

    virtual int16_t loadInt16() {
        int16_t result;
        loadBuffer(&result, sizeof(result));
        return result;
    }

    virtual int32_t loadInt32() {
        int32_t result;
        loadBuffer(&result, sizeof(result));
        return result;
    }

    virtual float loadFloat() {
        float result;
        loadBuffer(&result, sizeof(result));
        return result;
    }

    virtual Vector3f loadVector3() {
        Vector3f result;
        loadBuffer(&result, sizeof(result));
        return result;
    }

    virtual Color loadRGB() {
        float r = loadFloat();
        float g = loadFloat();
        float b = loadFloat();
        return Color::fromRGBAf(r, g, b, 1.0f);
    }

    virtual Color loadRGBA() {
        float r = loadFloat();
        float g = loadFloat();
        float b = loadFloat();
        float a = loadFloat();
        return Color::fromRGBAf(r, g, b, a);
    }
};

}

#endif //ASSEMBLE_MMDBUFFERDATALOADER_HPP
