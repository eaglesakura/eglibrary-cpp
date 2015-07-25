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


}
}