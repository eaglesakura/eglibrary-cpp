#pragma once

/**
 * BUILD_XXXでプラットフォームを示す。
 *
 * BUILD_Android
 * BUILD_iOS
 * BUILD_MacOSX
 */
#if defined(__ANDROID__)
#   define BUILD_Android
#elif defined(__MACH__)
#   define BUILD_MacOSX
#elif defined(__APPLE__)
#   define BUILD_iOS
#else
#error  Unknown Platform
#endif


#ifndef NDEBUG
#define DEBUG
#endif
