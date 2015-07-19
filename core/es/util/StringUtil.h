#pragma once

#include    "es/eglibrary.hpp"
#include    <vector>
#include    <es/memory/SafeArray.hpp>

namespace es {

class StringUtils {
public:
    /**
     * printf書式のフォーマットを指定して生成する
     */
    static std::string format(const char *fmt, ...);

    /**
     * printf書式のフォーマットを指定して生成する
     */
    static std::string format(const uint workingBufferBytes, const char *fmt, ...);

    /**
     * 文字列を適当なseparatorで分割する
     */
    static int split(const std::string &origin, const std::string &delim, std::vector<std::string> *result);
};

}
