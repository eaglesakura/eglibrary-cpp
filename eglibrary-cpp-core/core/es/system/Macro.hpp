#ifndef es_system_MACRO_HPP_
#define es_system_MACRO_HPP_

#ifdef __ANDROID__
#undef  __APPLE__   // guard IDE
#endif

/**
 * BUILD_XXXでプラットフォームを示す。
 *
 * BUILD_Android
 * BUILD_iOS
 * BUILD_MacOSX
 */
#ifndef BUILD_MacOSX
// 定数設定
#ifdef  __APPLE__
#define BUILD_iOS 1
#endif

// ANDROIDモード
#ifdef  __ANDROID__
#ifndef BUILD_Android
#define BUILD_Android 1
#endif
#endif
#endif

#ifndef BUILD_Android /* DEBUG CHECK */
/**
 * NDK_DEBUG=1の場合、NDEBUGフラグが立たないためDEBUGを立てる。
 */
#ifndef NDEBUG
#ifndef DEBUG
#define DEBUG
#endif
#endif

#else

#ifndef NDEBUG
#define DEBUG
#endif

#endif /* DEBUG CHECK */

/**
 * for IDE(Eclipse)
 */

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
#define     SAFE_DELETE( p )        if( p ){    delete p; p = NULL;     }
#endif /* SAFE_DELETE */

/**
 * 安全に配列deleteを行う
 */
#ifndef SAFE_DELETE_ARRAY
#define     SAFE_DELETE_ARRAY( p )  if( p ){    delete[] p; p = NULL;   }
#endif /* SAFE_DELETE_ARRAY */

/**
 * ２つの変数の値を交換する
 */
template<typename T>
inline void swap(T *a, T *b) {
    T temp = *a;
    *a = *b;
    *b = temp;
}

#endif
