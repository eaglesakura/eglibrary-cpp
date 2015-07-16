#pragma once

#include    "es/Graphics.hpp"
#include    "es/graphics/gl/context/DeviceContext.h"
#include    "es/graphics/gl/GLObject.h"

namespace es {

class ShaderProgram : public GLObject {
    GLuint program;
    uint16_t maxAttributes;
    uint16_t maxUniforms;

    ShaderProgram(const GLuint program);

public:
    virtual ~ShaderProgram();

    /**
     * 管理しているリソースの解放を行う
     */
    virtual void dispose();

    /**
     * シェーダープログラムのハンドラを取得する
     */
    GLuint getProgramHandle() const;

    /**
     * プログラム属性変数のロケーションを取得する
     */
    GLint getAttribLocation(const char *name) const;

    /**
     * uniform変数のロケーションを取得する
     */
    GLint getUniformLocation(const char *name) const;

    /**
     * GLと関連付ける
     */
    virtual void bind();

    /**
     * ビルドを行う
     */
    static ::std::shared_ptr<ShaderProgram> build(const char *vertex_shader, const char *frament_shader, MDeviceContext state = MDeviceContext());
};

}
