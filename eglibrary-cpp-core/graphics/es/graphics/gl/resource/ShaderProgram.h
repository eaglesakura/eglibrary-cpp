#ifndef es_graphics_gl_SHADERPROGRAM_H_
#define es_graphics_gl_SHADERPROGRAM_H_

#include    "es/Graphics.hpp"
#include    "es/graphics/gl/context/ThreadDevice.h"
#include    "es/graphics/gl/GLObject.h"

namespace es {

class ShaderProgram: public GLObject {
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
     * プログラム属性変数のロケーションを取得する
     */
    virtual GLint getAttribLocation(const char *name) const;

    /**
     * uniform変数のロケーションを取得する
     */
    virtual GLint getUniformLocation(const char *name) const;

    /**
     * GLと関連付ける
     */
    virtual void bind(MThreadDevice state = MThreadDevice());

    /**
     * ビルドを行う
     */
    static std_shared_ptr<ShaderProgram> build(const char* vertex_shader, const char* frament_shader, MThreadDevice state = MThreadDevice());
};

/**
 * managed
 */
typedef std_shared_ptr<ShaderProgram> MShaderProgram;

}

#endif /* SHADERPROGRAM_HPP_ */
