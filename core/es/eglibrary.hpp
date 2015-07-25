#pragma once

#include    "es/debug/Testing.hpp"
#include    <stdint.h>
#include    <limits.h>
#include    <stddef.h>
#include    <stdlib.h>
#include    <string.h>
#include    <string>

#include    <cstddef>
#include    <cstdio>
#include    <mutex>
#include    <thread>

#include    <memory>


#include	"es/system/Macro.hpp"
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
