#ifdef __ANDROID__
#pragma once

#include "es/system/string/IStringConverter.h"
#include <jni.h>
#include "JointConnector.hpp"

namespace es {
namespace internal {

/**
 * JavaのString.getBytesを経由してUTF8->UTF32LE変換を行う
 */
class JavaStringConverterImpl : public IStringConverter {
    struct {
        jmethodID method_getBytes = nullptr;
    } javaString;

    jc::lang::string_wrapper UTF32LE;

    void initialize();

public:
    JavaStringConverterImpl();

    virtual ~JavaStringConverterImpl();

    /**
     * UTF8文字列をワイド文字列に変換する
     */
    virtual std::wstring toWideString(const std::string &utf8);
};

}
}

#endif /* __ANDROID__ */