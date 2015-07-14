#ifndef es_system_LOG_H_
#define es_system_LOG_H_

#include    <string.h>

namespace es {

namespace internal {

/**
 * __FILE__マクロからファイル名を取得する
 */
inline char *__getFileName(const char *__file__) {
#if (defined(BUILD_MacOSX) || defined(BUILD_Android) || defined(BUILD_MacOSX) )
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

class Logger {
public:
    /**
     * libMikuMikuSimple用のログ関数
     * デフォルトログ関数を利用しない場合、適宜書き換える。
     */
    typedef void (*LogFunctionPtr)(const LogType_e type, const char *, const char *, ...);

private:
    static LogFunctionPtr func;
public:
    /**
     * ログの実装を変更する
     */
    static void setFunction(LogFunctionPtr ptr) {
        Logger::func = ptr;
    }

    /**
     * ログの実装を取得する
     */
    static LogFunctionPtr get() {
        return func;
    }
};

}

#if !defined(EGLIBRARY_NO_LOG)

/**
 * 特定条件下の設定
 */
#define eslog_from(file, line, ...)       { ::es::internal::Logger::get()(::es::internal::LogType_Debug, ::es::internal::__getFileName(file),  "L " es_num_to_str(__LINE__) " | " __VA_ARGS__); }

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
#define eslog(...)       ::es::internal::Logger::get()(::es::internal::LogType_Info, ::es::internal::__getFileName(__FILE__), "L " es_num_to_str(__LINE__) " | " __VA_ARGS__)

/**
 * フォーマット付きログ
 */
#define esdebug(...)     ::es::internal::Logger::get()(::es::internal::LogType_Debug, ::es::internal::__getFileName(__FILE__), "L " es_num_to_str(__LINE__) " | " __VA_ARGS__)

/**
 * アラート表示
 */
#define esalert(...)      ::es::internal::Logger::get()(::es::internal::LogType_Alert, ::es::internal::__getFileName(__FILE__), "L %d | " __VA_ARGS__)

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
