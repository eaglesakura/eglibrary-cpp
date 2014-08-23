#include    "es/eglibrary.hpp"

#ifdef  BUILD_Android
#include    "android/log.h"
#endif

namespace es {

void __logDebugF(const LogType_e type, const char* __file, const char* fmt, ...) {
#if !defined(EGLIBRARY_NO_LOG) /* opt out EGLIBRARY_NO_LOG */

// debug check
#if !defined(DEBUG)
    // デバッグモード以外ならログレベルをチェック
    if(type >= LogType_Debug) {
        return;
    }
#endif

    /**
     * Android用ビルド
     */
#ifdef  BUILD_Android
    va_list ap;
    va_start(ap, fmt);

    static const int LOG_TYPES[] = { ANDROID_LOG_INFO, ANDROID_LOG_DEBUG, ANDROID_LOG_ERROR, };
    __android_log_vprint(LOG_TYPES[type], __file, fmt, ap);

    va_end(ap);
#endif  /* BUILD_Android */

    /**
     * その他のプラットフォーム
     */
#if defined(BUILD_iOS) || defined(BUILD_MacOSX)
    va_list ap;
    va_start(ap, fmt);

    printf("%s | ", __file); // ファイル名出力
    vprintf(fmt, ap); // 通常ログ出力
    printf("\n"); // コンソール改行

    va_end(ap);
#endif

#endif /* opt out EGLIBRARY_NO_LOG  */
}

}
