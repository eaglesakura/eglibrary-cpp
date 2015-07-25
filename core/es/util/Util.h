#pragma once

#include "es/eglibrary.hpp"
#include <map>

namespace es {
namespace util {

/**
 * ２つの変数の値を交換する
 */
template<typename T>
inline void swap(T *a, T *b) {
    T temp = *a;
    *a = *b;
    *b = temp;
}

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