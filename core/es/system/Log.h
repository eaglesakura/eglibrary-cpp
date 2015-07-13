#ifndef es_system_LOG_H_
#define es_system_LOG_H_

#include    <string.h>

namespace es {

/**
 * __FILE__マクロからファイル名を取得する
 */
inline char* __getFileName(const char* __file__) {
#if ( defined(BUILD_MacOSX) || defined(BUILD_Android) || defined(BUILD_MacOSX) )
    return strrchr(__file__, '/') + 1;
#else
    return strrchr(__file__, '/') + 1;
#endif
}

enum LogType_e {
    /* 通常出力 */
    LogType_Info,

    /* デバッグ中のみ表示したい */
    LogType_Debug,

    /* 特に重要なログで表示する */
    LogType_Alert,
};

#ifndef LOG_TAG
#define LOG_TAG "jni-log"
#endif

void __logDebugF(const LogType_e type, const char* __file, const char* fmt, ...);

#if !defined(EGLIBRARY_NO_LOG)

/**
 * 特定条件下の設定
 */
#define eslog_from(file, line, ... )       { ::es::__logDebugF(::es::LogType_Debug, ::es::__getFileName(file),  "L " es_num_to_str(__LINE__) " | " __VA_ARGS__); }

/**
 * 文字列化用ダミー
 */
#define _dummy_es_num_to_str(num) #num

/**
 * 文字列化する
 */
#define es_num_to_str(num) _dummy_es_num_to_str(num)

/**
 * フォーマット付きログ
 */
#define eslog(... )       ::es::__logDebugF(::es::LogType_Info, ::es::__getFileName(__FILE__), "L " es_num_to_str(__LINE__) " | " __VA_ARGS__)

/**
 * フォーマット付きログ
 */
#define esdebug(... )       ::es::__logDebugF(::es::LogType_Debug, ::es::__getFileName(__FILE__), "L " es_num_to_str(__LINE__) " | " __VA_ARGS__)

/**
 * アラート表示
 */
#define esalert( ... )      ::es::__logDebugF(::es::LogType_Alert, ::es::__getFileName(__FILE__), "L %d | " __VA_ARGS__)

#else

/**
 * 特定条件下の設定
 */
#define eslog_from(...)       {  }

/**
 * 単純ログ
 */
#define eslog( ... )            {  }

/**
 * デバッグログ
 */
#define esdebug(... )   {   }

/**
 * アラート表示
 */
#define esalert( ... )      {  }

#endif

}

#endif /* es_system_LOG_H_ */
