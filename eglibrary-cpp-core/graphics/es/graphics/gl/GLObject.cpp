#include    "GLObject.h"
#include    "es/graphics/gl/context/ThreadDevice.h"

namespace es {

MShaderState GLObject::get_state(MShaderState state) {
    if (state) {
        return state;
    } else {
        return ThreadDevice::current()->getShaderState();
    }
}
MThreadDevice GLObject::get_device(MThreadDevice device) {
    if (device) {
        return device;
    } else {
        return ThreadDevice::current();
    }
}

MRenderState GLObject::get_render(MRenderState state) {
    if (state) {
        return state;
    } else {
        return ThreadDevice::current()->getRenderState();
    }
}

}
