#pragma once

#include "es/eglibrary.hpp"

namespace es {

class IStringConverter {
public:
    virtual ~IStringConverter() = default;

    /**
     * UTF8文字列をワイド文字列に変換する
     */
    virtual std::wstring toWideString(const std::string &utf8) = 0;
};

}