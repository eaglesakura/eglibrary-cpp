#pragma once

#if defined(ES_BUILD_GTEST_MODE)

#include <assert.h>
#include <exception>
#include <string>
#include "gtest/gtest.h"

namespace es {
namespace debug {

class TestAssertException : public std::exception {
    std::string whatText;
public:
    TestAssertException(const char *__file__, const int __line__, const char *__whatText) {
        whatText = std::string(__file__) +
                   std::string(" | L ") +
                   std::to_string(__line__) +
                   std::string("\nERROR : ") +
                   std::string(__whatText) +
                   std::string("\n");
    }

    virtual const char *what() const _NOEXCEPT {
        return whatText.c_str();
    }
};

}
}

#ifdef  assert
#undef  assert

/**
 * gtestの実行は強制停止ではなく、例外を投げてgtestに挙動をハンドリングさせる
 */
#define assert(e)  if(!(e)){ throw ::es::debug::TestAssertException(__FILE__, __LINE__, #e); }
#endif

#endif