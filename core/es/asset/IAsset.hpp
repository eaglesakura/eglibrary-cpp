#pragma once

#include "es/eglibrary.hpp"
#include "es/memory/Buffer.hpp"

namespace es {
class IAsset {
public:
    virtual ~IAsset() = default;

    /**
     * 次のサイズを読み込む
     *
     * リクエストされたsizeの容量を読み込むように努めるが、ファイル終端になった場合はsize以下の値を返却する
     */
    virtual unsafe_array<uint8_t> read(const uint size) = 0;

    /**
     * 残容量を取得する
     */
    virtual uint available() const = 0;
};

namespace util {

/**
 * アセットを文字列として読み込む
 */
std::string toString(std::shared_ptr<IAsset> &asset);

/**
 * byte配列に変換する
 */
ByteBuffer toByteArray(std::shared_ptr<IAsset> &asset);

/**
 * 入力されたassetをメモリ上に完全コピーして、アセットを廃棄可能にする。
 */
std::shared_ptr<IAsset> toOnMemory(std::shared_ptr<IAsset> &asset);

}

}