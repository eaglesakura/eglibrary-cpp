#pragma once

#include    "es/graphics/gl/context/DeviceContext.h"

namespace es {

class GLObject : public Object {
protected:
    GLObject() {
    }

    static MDeviceContext get_device(MDeviceContext device = MDeviceContext());

    static MRenderState get_render(MRenderState state = MRenderState());

    static MRenderState get_render(MDeviceContext device = MDeviceContext()) {
        return get_device(device)->getRenderState();
    }

    static MShaderState get_state(MShaderState state = MShaderState());

    static MShaderState get_state(MDeviceContext device = MDeviceContext()) {
        return get_device(device)->getShaderState();
    }

public:
    virtual ~GLObject() {
    }

    /**
     * 解放を行う
     */
    virtual void dispose() = 0;
};

}
