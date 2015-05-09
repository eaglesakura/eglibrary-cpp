#include    "StringUtil.h"
#include <sstream>

using namespace std;

namespace es {

/**
 * 文字列を適当なseparatorで分割する
 */
int StringUtils::split(const std::string &origin, const std::string &delim, std::vector<std::string> *result) {
    int oldSize = result->size();

    istringstream iss(origin);
    copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter(*result));
    return result->size() - oldSize;
}

std::string StringUtils::format(const char *fmt, ...) {
    std_shared_ptr<char> temp(new char[strlen(fmt) + 256]);
    va_list ap;
    va_start(ap, fmt);
    {
        vsprintf(temp.get(), fmt, ap);
    }
    va_end(ap);
    return std::string(temp.get());
}
}
