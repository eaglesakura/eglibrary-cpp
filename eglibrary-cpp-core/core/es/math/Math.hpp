#ifndef MATH_HPP_
#define MATH_HPP_

#include    "es/eglibrary.hpp"
#include    <math.h>
#include    <cstdlib>

namespace es {

/**
 * 値が_min <= value <= _maxになるように正規化してvalueを返す。
 */
template<class T>
inline const T minmax(const T _min, const T _max, const T value) {
    if (value < _min) {
        return _min;
    }

    if (value > _max) {
        return _max;
    }

    return value;
}

/**
 * 整数nが2の乗数の場合、trueを返す。
 */
template<class T>
inline bool isPowerOfTwo(const T n) {
    if (!n) {
        return true;
    }

    T bit = 1;
    for (int i = 0; i < 32; ++i) {
        if (n == bit) {
            return true;
        }
        bit <<= 1;
    }

    return false;
}

/**
 * サイズが2のn乗になるように修正する
 */
inline uint toPowerOfTwo(const uint size) {
    uint result = 1;

    while (result < size) {
        result <<= 1;
    }

    return result;
}

}

#endif /* MATH_HPP_ */
