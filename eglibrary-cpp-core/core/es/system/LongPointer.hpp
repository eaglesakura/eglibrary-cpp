/*
 * LongPointer.hpp
 *
 *  Created on: 2014/08/23
 *      Author: eaglesakura
 */

#ifndef es_jc_LONGPOINTER_HPP_
#define es_jc_LONGPOINTER_HPP_

#include    "es/eglibrary.hpp"
#include    <memory>

namespace es {

/**
 * 整数変換したポインタ
 */
typedef int64_t long_shared_pointer;

/**
 * 整数変換したポインタを扱う。
 * 主にNDKのブリッジで使用する。
 */
class LongShraredPtr {
public:
    template<typename T>
    static long_shared_pointer create(const std_shared_ptr<T> p) {
        std_shared_ptr<T> *ptr = new std_shared_ptr<T>(p);
        return reinterpret_cast<long_shared_pointer>(ptr);
    }

    template<typename T>
    static std_shared_ptr<T> get(const long_shared_pointer lp) {
        std_shared_ptr<T> *ptr = reinterpret_cast< std_shared_ptr<T>*>(lp);
        if (ptr) {
            return *ptr;
        } else {
            return std_shared_ptr < T > (nullptr);
        }
    }

    /**
     * 新たな参照を生成する
     */
    template<typename T>
    static long_shared_pointer copy(const long_shared_pointer lp) {
        std_shared_ptr<T> p = get<T>(lp);
        return create<T>(p);
    }

    template<typename T>
    static void dispose(const long_shared_pointer lp) {
        if (!lp) {
            return;
        }

        std_shared_ptr<T> *ptr = reinterpret_cast<std_shared_ptr<T>*>(lp);
        SAFE_DELETE(ptr);
    }
};

/**
 * ポインタを取得する
 */
#define es_longpointer(type, ptr)   ::es::LongShraredPtr::get< type >(ptr)

}

#endif /* LONGPOINTER_HPP_ */
