#ifndef es_util_STRINGUTIL_H_
#define es_util_STRINGUTIL_H_

#include    "es/eglibrary.hpp"
#include    <vector>

namespace es {

class StringUtils {
public:
    /**
     * 文字列を適当なseparatorで分割する
     */
    static int split(const std::string &origin, const std::string &delim, std::vector<std::string> *result);
};

}

#endif /* STRINGUTIL_HPP_ */
