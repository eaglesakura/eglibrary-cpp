#pragma once

#include "es/asset/IAsset.hpp"
#include <vector>
#include <fstream>

namespace es {

namespace internal {

/**
 * 一旦メモリに全て読み込むアセット
 */
class InMemoryAsset : public Object, public IAsset {
    /**
     * メモリに読み込んだデータ
     */
    std::vector<uint8_t> buffer;

    /**
     * 現在指しているオフセット値
     */
    uint offset = 0;

public:
    InMemoryAsset(std::ifstream &stream) {
        stream.seekg(0, std::ifstream::end);
        uint bufferSize = stream.tellg();
        stream.clear();
        stream.seekg(0, std::ifstream::beg);

        // バッファを確保する
        util::valloc(&buffer, bufferSize, false);
        stream.read((char *) util::asPointer(buffer), bufferSize);

        stream.close();
#if 0
        uint8_t buf;
        while (!stream.eof()) {
            stream.read((char *) &buf, 1);
            buffer.push_back(buf);
        }
#endif
    }

    virtual ~InMemoryAsset() = default;


    /**
     * 次のサイズを読み込む
     *
     * リクエストされたsizeの容量を読み込むように努めるが、ファイル終端になった場合はsize以下の値を返却する
     */
    virtual unsafe_array<uint8_t> read(const uint size) {

        uint sliceSize = std::min(size, available());
        assert(sliceSize <= buffer.size());

        if (sliceSize == 0) {
            return unsafe_array<uint8_t>();
        }

        unsafe_array<uint8_t> result(util::asPointer(buffer, offset), sliceSize);
        offset += size;

        return result;
    }

    /**
     * 残容量を取得する
     */
    virtual uint available() const {
        assert(offset <= buffer.size());
        return (uint) (buffer.size()) - offset;
    }
};

}
}


