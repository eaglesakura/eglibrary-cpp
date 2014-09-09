#ifndef gl_graphics_gl_GLOBJECT_H_
#define gl_graphics_gl_GLOBJECT_H_

#include    "es/graphics/gl/context/ThreadDevice.h"

namespace es {

class GLObject: public Object {
protected:
    GLObject() {
    }

    static MThreadDevice get_device(MThreadDevice device = MThreadDevice());
    static MRenderState get_render(MRenderState state = MRenderState());
    static MShaderState get_state(MShaderState state = MShaderState());
public:
    virtual ~GLObject() {
    }

    /**
     * 解放を行う
     */
    virtual void dispose() = 0;
};

}

#endif /* GLOBJECT_HPP_ */
