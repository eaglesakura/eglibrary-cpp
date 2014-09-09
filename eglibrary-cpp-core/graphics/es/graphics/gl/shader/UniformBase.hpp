#ifndef es_grahpics_gl_UNIFORMBASE_HPP_
#define es_grahpics_gl_UNIFORMBASE_HPP_

#include    "es/Graphics.hpp"
#include    "es/graphics/gl/resource/ShaderProgram.h"

namespace es {

class UniformBase {
protected:
    /**
     * Uniformインデックス
     */
    GLint location;

    UniformBase() {
        location = -1;
    }
public:

    ~UniformBase() {
    }

    /**
     * Uniform値が有効ならばtrueを返す
     */
    bool valid() const {
        return location >= 0;
    }

    /**
     * ロケーション設定を行う
     */
    void setLocation(const GLint location) {
        this->location = location;
    }

    /**
     * Uniformロケーションを取得する。
     * 無効な場合、UNIFORM_DISABLE_INDEXが帰る.
     */
    GLint getLocation() const {
        return location;
    }

    /**
     * uniform locationを取得する
     */
    bool setLocation(const MShaderProgram program, const char* name) {
        location = program->getUniformLocation(name);
        return valid();
    }
};

}

#endif /* UNIFORMBASE_HPP_ */
