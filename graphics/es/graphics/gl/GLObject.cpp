#include    "GLObject.h"
#include    "es/graphics/gl/context/DeviceContext.h"

namespace es {

MShaderState GLObject::get_state(MShaderState state) {
    if (state) {
        return state;
    } else {
        return DeviceContext::current()->getShaderState();
    }
}
MDeviceContext GLObject::get_device(MDeviceContext device) {
    if (device) {
        return device;
    } else {
        return DeviceContext::current();
    }
}

MRenderState GLObject::get_render(MRenderState state) {
    if (state) {
        return state;
    } else {
        return DeviceContext::current()->getRenderState();
    }
}

}
