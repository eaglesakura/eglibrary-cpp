#ifndef BUFFER_HPP_
#define BUFFER_HPP_

#include    "es/eglibrary.hpp"
#include    <malloc.h>

namespace es {

class ByteBuffer {

    /**
     * 管理しているバッファ
     */
    std_shared_ptr<uint8_t> buffer;

    /**
     * バッファのサイズ
     */
    uint _length;
public:
    ByteBuffer() {
        _length = 0;
    }

    ByteBuffer(std_shared_ptr<uint8_t> buffer, const uint _len) {
        this->buffer = buffer;
        this->_length = _len;
    }

    ByteBuffer(const ByteBuffer& cpy) {
        this->buffer = cpy.buffer;
        this->_length = cpy._length;
    }

    ~ByteBuffer() {

    }

    uint8_t* get() const {
        return buffer.get();
    }

    uint length() const {
        return _length;
    }

    bool empty() const {
        return _length == 0;
    }
};

class Buffer {
    Buffer();
    ~Buffer();
public:

    /**
     * バッファを確保する
     */
    static ByteBuffer create(uint bytes) {
        return ByteBuffer( std_shared_ptr<uint8_t>(static_cast<uint8_t*>(malloc(bytes)), free), bytes);
    }

    /**
     * 0クリアされたバッファを生成する
     */
    static ByteBuffer createZeroBuffer(uint bytes) {
        return ByteBuffer(std_shared_ptr<uint8_t>(static_cast<uint8_t*>(calloc(1, bytes)), free), bytes);
    }
};

}

#endif /* BUFFER_HPP_ */
