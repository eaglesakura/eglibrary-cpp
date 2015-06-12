#ifndef es_util_STRINGUTIL_H_
#define es_util_STRINGUTIL_H_

#include    "es/eglibrary.hpp"
#include    <vector>
//#include    <iconv.h>
#include <es/memory/SafeArray.hpp>

namespace es {

#if 0
static const char *CHARSET_SJIS = "SJIS";
static const char *CHARSET_UTF8 = "UTF-8";
static const char *CHARSET_UTF16LE = "UTF-16LE";

class StringEncoder {
    ::iconv_t ic;
public:

    StringEncoder(const char *dstFormat, const char *srcFormat) {
        ic = ::iconv_open(dstFormat, srcFormat);
    }

    ~StringEncoder() {
        if (ic) {
            ::iconv_close(ic);
        }
    }

    std::string encode(const uint8_t *srcBuffer, int bufferSize = -1) {
        if (bufferSize <= 0) {
            bufferSize = strlen((char *) srcBuffer) * 3;
        }

        safe_array<uint8_t> inbuf(bufferSize + 1);
        safe_array<uint8_t> outbuf(bufferSize + 1);

        inbuf.zeromemory();
        outbuf.zeromemory();

        char *in = (char *) inbuf.ptr;
        char *out = (char *) outbuf.ptr;
        size_t in_size = (size_t) bufferSize;
        size_t out_size = (size_t) bufferSize;
        memcpy(in, srcBuffer, strlen((char *) srcBuffer));

        iconv(ic, &in, &in_size, &out, &out_size);

        return std::string((char *) outbuf.ptr);
    }
};
#endif

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

#endif /* STRINGUTIL_HPP_ */
