#include    "es/eglibrary.hpp"

#if defined(BUILD_Android)
#include    "android/log.h"

namespace {

/**
 * ログ出力を行う
 */
void logAndroid(const es::internal::LogType_e type, const char *__file, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);

    static const int LOG_TYPES[] = { ANDROID_LOG_INFO, ANDROID_LOG_DEBUG, ANDROID_LOG_ERROR, };
    __android_log_vprint(LOG_TYPES[type], __file, fmt, ap);

    va_end(ap);
}

}

namespace es {
internal::Logger::LogFunctionPtr internal::Logger::func = logAndroid;
}

#elif !defined(BUILD_iOS)

namespace {

/**
 * ログ出力を行う
 */
void logBasic(const es::internal::LogType_e type, const char *__file, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);

    printf("%s | ", __file); // ファイル名出力
    vprintf(fmt, ap); // 通常ログ出力
    printf("\n"); // コンソール改行

    va_end(ap);
}

}
namespace es {
internal::Logger::LogFunctionPtr internal::Logger::func = logBasic;
}

#endif

