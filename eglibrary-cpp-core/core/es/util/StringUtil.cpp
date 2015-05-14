#include    "StringUtil.h"
#include <sstream>
#include <es/memory/Buffer.hpp>

using namespace std;

namespace es {

/**
 * 文字列を適当なseparatorで分割する
 */
int StringUtils::split(const std::string &origin, const std::string &delim, std::vector<std::string> *result) {
    int oldSize = (int) result->size();

    istringstream iss(origin);
    copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter(*result));
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

}
