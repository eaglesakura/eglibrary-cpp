#include "IAsset.hpp"
#include <vector>

namespace es {
namespace util {


/**
 * アセットを文字列として読み込む
 */
std::string toString(const std::shared_ptr<IAsset> &asset) {
    auto buffer = asset->read(asset->available());

    assert(!asset->available()); // 末尾まで読み込めている

    std::vector<uint8_t> temp(buffer.length + 1);
    util::zeromemory(&temp);

    memcpy(asPointer(temp), buffer.ptr, buffer.length);

    return std::string((char *) asPointer(temp));
}

/**
 * byte配列に変換する
 */
ByteBuffer toByteArray(const std::shared_ptr<IAsset> &asset) {
    auto buffer = asset->read(asset->available());
    ByteBuffer result = Buffer::create(buffer.length);
    memcpy(result.get(), buffer.ptr, buffer.length);
    return result;
}


}
}
