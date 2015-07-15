#pragma once

#include    "es/eglibrary.hpp"
#include    "es/Graphics.hpp"
#include    "es/graphics/Color.hpp"

namespace es {

class ColorUniform: public UniformBase {
    /**
     * 転送済みの色
     */
    rgba32 color;
public:
    ColorUniform() {
        color = 0;
    }

    ColorUniform(std::shared_ptr<ShaderProgram> program, const char *name) {
        color = 0;
        setLocation(program, name);
    }

    ~ColorUniform() {
    }

    /**
     * 色情報を転送する
     */
    bool upload(const Color &c) {
        if (!valid()) {
            return false;
        }

        if (color != c.rgba) {
//            eslog("upload loc(%d) r(%f) g(%f) b(%f), a(%f)", location, c.rf(), c.gf(), c.bf(), c.af());
            glUniform4f(location, c.rf(), c.gf(), c.bf(), c.af());
            assert_gl();
            color = c.rgba;
            return true;
        }
        return false;
    }

    /**
     * 色情報を転送する
     */
    bool upload(const rgba32 c) {
        return upload(Color(c));
    }
};
}

