#include    "StringUtil.h"
#include <iterator>
#include <sstream>
#include <es/memory/Buffer.hpp>
//#include <codecvt>

namespace es {

/**
 * 文字列を適当なseparatorで分割する
 */
int StringUtils::split(const std::string &origin, const std::string &delim, std::vector<std::string> *result) {
    const size_t oldSize = result->size();

    std::istringstream iss(origin);
    copy(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>(), std::back_inserter(*result));
    return result->size() - oldSize;
}

std::string StringUtils::format(const char *fmt, ...) {
//    return std::string("TEST");
    ByteBuffer buffer = Buffer::createZeroBuffer(strlen(fmt) + 256);

    va_list ap;
    va_start(ap, fmt);
    vsprintf((char *) buffer.get(), fmt, ap);
    va_end(ap);
    return std::string((char *) buffer.get());
}

/**
 * printf書式のフォーマットを指定して生成する
 */
std::string StringUtils::format(const uint workingBufferBytes, const char *fmt, ...) {
    ByteBuffer buffer = Buffer::createZeroBuffer(strlen(fmt) + workingBufferBytes);

    va_list ap;
    va_start(ap, fmt);
    vsprintf((char *) buffer.get(), fmt, ap);
    va_end(ap);
    return std::string((char *) buffer.get());
}

namespace util {

//std::wstring toWideString(const char *utf8) {
//    static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> conversion;
//    return conversion.from_bytes(utf8);
//}

//std::wstring toWideString(const std::string &utf8) {
//    return toWideString(utf8.c_str());
//}

}

}
