#pragma once

#include    "es/protoground-types.hpp"
#include    "es/debug/Testing.hpp"
#include    <stdint.h>
#include    <string.h>
#include    <string>
#include    <mutex>
#include    <thread>
#include    <memory>
#include    <sys/types.h>

#include	"es/system/Log.h"
#include    "es/system/Object.hpp"
#include    "es/memory/selection_ptr.hpp"

namespace es {
template<typename T>
using sp = ::std::shared_ptr<T>;

template<typename T>
using wp = ::std::weak_ptr<T>;

template<typename T>
using selp  = ::es::selection_ptr<T>;


typedef std::recursive_mutex es_mutex;
typedef std::unique_lock<es_mutex> es_mutex_lock;

typedef std::string string;
typedef std::wstring wide_string;
typedef int64_t wide_char;

}
