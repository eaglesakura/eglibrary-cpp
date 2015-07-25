#pragma once

#include <map>

namespace es {
namespace util {

/**
 * Mapから簡易的に値を探す
 */
template<typename keyType, typename valueType>
inline ::std::shared_ptr<valueType> find(const std::map<keyType, ::std::shared_ptr<valueType> > &objMap, const keyType &key) {
    auto itr = objMap.find(key);
    if (itr != objMap.end()) {
        // found
        return itr->second;
    } else {
        // notfound
        return ::std::shared_ptr<valueType>();
    }
}

}
}