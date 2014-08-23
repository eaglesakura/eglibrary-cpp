/*
 * LongPointer.hpp
 *
 *  Created on: 2014/08/23
 *      Author: eaglesakura
 */

#ifndef es_jc_LONGPOINTER_HPP_
#define es_jc_LONGPOINTER_HPP_

#include    "jointconnector.hpp"
#include    "es/eglibrary.hpp"

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
    static long_shared_pointer create(const ::std::shared_ptr<T> p) {
        ::std::shared_ptr<T> *ptr = new ::std::shared_ptr<T>(p);
        return reinterpret_cast<long_shared_pointer>(ptr);
    }

    template<typename T>
    static ::std::shared_ptr<T> get(const long_shared_pointer lp) {
        ::std::shared_ptr<T> *ptr = reinterpret_cast< ::std::shared_ptr<T>* >(lp);
        if (ptr) {
            return *ptr;
        } else {
            return ::std::shared_ptr<T>(nullptr);
        }
    }

    /**
     * 新たな参照を生成する
     */
    template<typename T>
    static long_shared_pointer copy(const long_shared_pointer lp) {
        ::std::shared_ptr<T> p = get<T>(lp);
        return create<T>(p);
    }

    template<typename T>
    static void dispose(const long_shared_pointer lp) {
        if (!lp) {
            return;
        }

        ::std::shared_ptr<T> *ptr = reinterpret_cast< ::std::shared_ptr<T>* >(lp);
        SAFE_DELETE(ptr);
    }
};

/**
 * ポインタを取得する
 */
#define es_longpointer(type, ptr)   ::es::LongShraredPtr::get< type >(ptr)

#if 0
/**
 * ポインタを取得する
 */
#define jc_pointer_get(field_name, type) ::es::jc::LongShraredPtr::get< type >(field_##field_name())

/**
 * long pointerをセットする
 */
#define jc_pointer_set(field_name, type, ptr) { \
                ::es::LongShraredPtr::dispose<type>( field_##field_name() );    \
                field_##field_name( ptr ? ::es::jc::LongShraredPtr::create< type >(ptr) : 0 ); \
            }
#endif

}

#endif /* LONGPOINTER_HPP_ */
