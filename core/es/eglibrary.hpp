#pragma once

#include    <stdint.h>
#include    <limits.h>
#include    <stddef.h>
#include    <stdlib.h>
#include    <assert.h>
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
#include    "es/memory/SelectionPtr.hpp"

namespace es {
typedef std::recursive_mutex es_mutex;
typedef std::unique_lock<es_mutex> es_mutex_lock;
}
