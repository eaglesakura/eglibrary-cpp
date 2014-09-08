/*
 * eglibrary.hpp
 *
 *  Created on: 2014/08/23
 *      Author: eaglesakura
 */

#ifndef EGLIBRARY_HPP_
#define EGLIBRARY_HPP_

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

namespace es {
typedef std::recursive_mutex es_mutex;
typedef std::unique_lock<es_mutex> es_mutex_lock;

}

#endif /* EGLIBRARY_HPP_ */
