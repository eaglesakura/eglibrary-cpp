#pragma once

#include <map>
#include <sys/types.h>

/**
 * BUILD_XXXでプラットフォームを示す。
 *
 * BUILD_Android
 * BUILD_iOS
 * BUILD_MacOSX
 */
#if defined(__ANDROID__)
#   define BUILD_Android
#elif defined(__MACH__)
#   define BUILD_MacOSX
#elif defined(__APPLE__)
#   define BUILD_iOS
#else
#error  Unknown Platform
#endif


#ifndef NDEBUG
#define DEBUG
#endif

/**
 * ::std::shared_ptrを認識しないIDE対策
 */
#ifndef std_shared_ptr
#define std_shared_ptr  ::std::shared_ptr
#endif

/**
 * ::std::weak_ptrを認識しないIDE対策
 */
#ifndef std_weak_ptr
#define std_weak_ptr    ::std::weak_ptr
#endif

/**
 * 安全にdeleteを行う
 */
#ifndef SAFE_DELETE
#define     SAFE_DELETE(p)        if( p ){    delete p; p = NULL;     }
#endif /* SAFE_DELETE */

/**
 * 安全に配列deleteを行う
 */
#ifndef SAFE_DELETE_ARRAY
#define     SAFE_DELETE_ARRAY(p)  if( p ){    delete[] p; p = NULL;   }
#endif /* SAFE_DELETE_ARRAY */

namespace es {

typedef unsigned int uint;

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
inline std_shared_ptr<valueType> find(const std::map<keyType, std_shared_ptr<valueType> > &objMap, const keyType &key) {
    auto itr = objMap.find(key);
    if (itr != objMap.end()) {
        // found
        return itr.second;
    } else {
        // notfound
        return std_shared_ptr<valueType>();
    }
}

}
