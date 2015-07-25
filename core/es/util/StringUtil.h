#pragma once

#include    "es/eglibrary.hpp"
#include    <vector>
#include    <es/memory/SafeArray.hpp>

namespace es {

namespace util {

/**
 * printf書式のフォーマットを指定して生成する
 *
 * バッファの大きさはstrlen(fmt) + 256を固定とするので、更に大きなバッファが必要な場合はバッファサイズ指定版を利用する必要がある。
 */
std::string format(const char *fmt, ...);

/**
 * 文字列をフォーマットする
 */
std::string format(const uint workingBufferBytes, const char *fmt, ...);

/**
 * 文字列を指定された分割文字に従って分割する
 */
int split(const string &origin, const string &delim, std::vector<string> *result);

}

}
