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
//        eslog("uniform location name(%s) -> %d", name, location);
        return valid();
    }
};

/**
 * boolean型の指定を行うuniform
 */
class BooleanUniform: public UniformBase {
    bool value;
public:
    BooleanUniform() {
        value = false;
    }

    BooleanUniform(MShaderProgram program, const char* name) {
        value = false;
        setLocation(program, name);
    }
    /**
     * 情報を転送する
     */
    bool upload(const bool set) {
        if (!valid()) {
            return false;
        }

        if (value != set) {
            glUniform1i(location, set ? 1 : 0);
            assert_gl();
            this->value = set;
            return true;
        }
        return false;
    }
};

}

#endif /* UNIFORMBASE_HPP_ */
