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

}
