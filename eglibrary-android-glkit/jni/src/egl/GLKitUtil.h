/*
 * GLKitUtil.h
 *
 *  Created on: 2014/08/22
 *      Author: eaglesakura
 */

#ifndef GLKITUTIL_H_
#define GLKITUTIL_H_

namespace es {
namespace glkit {

/**
 *
 */
class GLKitUtil {
public:
    static void captureSurfaceRGB565(void* dst);
    static void captureSurfaceRGBA8(void* dst);
    static void captureSurfaceRGBA4(void* dst);

    /**
     * サー
     */
    static void captureSurfaceA8(void* dst);
};

}
}

#endif /* GLKITUTIL_H_ */
