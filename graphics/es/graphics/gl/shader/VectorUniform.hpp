#pragma once

#include    "UniformBase.hpp"
#include    "es/math/Vector2.hpp"
#include    "es/math/Vector3.hpp"
#include    "es/math/Vector4.hpp"
#include    "es/math/Matrix.hpp"

namespace es {

/**
 * ベクトル型のUniformをサポートする
 * 無効なUniform値（コンパイラ最適化で削除された等）の場合はアップロードを無視する
 *
 * 転送されるとき、シェーダーのユニットはvec4単位で確保されるため、効率的に利用する場合はシェーダー側で適当に配置を考えてやる必要がある。
 * 上手いことスキマに配置してくれるハズだが、ユニット数の残量には気をつけること。
 */
template<typename vector_type, uint vector_length>
class VectorUniform: public UniformBase {
    /**
     * 保持するベクトルユニット
     */
    vector_type vec[vector_length];
public:
    VectorUniform() {
        // 0で初期化する
        for (int i = 0; i < vector_length; ++i) {
            vec[i] = (vector_type) 0;
        }
    }

    VectorUniform(::std::shared_ptr<ShaderProgram> program, const char *name) {
        // 0で初期化する
        for (int i = 0; i < vector_length; ++i) {
            vec[i] = (vector_type) 0;
        }

        setLocation(program, name);
    }

    ~VectorUniform() {
    }

    /**
     * 要素を取得する
     */
    vector_type get(const uint index) const {
        assert(vector_length > index);

        return vec[index];
    }

    /**
     * float値を1つ転送する
     */
    bool upload(const float x) {
        assert(vector_length >= 1);

        if (!valid()) {
            return false;
        }

        if (vec[0] != x) {
            glUniform1f(location, x);
            assert_gl();
            vec[0] = x;
            return true;
        }
        return false;
    }

    /**
     * float値を2つ転送する
     */
    bool upload(const float x, const float y) {
        assert(vector_length >= 2);

        if (!valid()) {
            return false;
        }

        if (vec[0] != x || vec[1] != y) {
            glUniform2f(location, x, y);
            assert_gl();
            vec[0] = x;
            vec[1] = y;
            return true;
        }
        return false;
    }

    /**
     * ベクトルを転送する
     */
    bool upload(const Vector2f &v) {
        return upload(v.x, v.y);
    }

    /**
     * float値を3つ転送する
     */
    bool upload(const float x, const float y, const float z) {
        assert(vector_length >= 3);

        if (!valid()) {
            return false;
        }

        if (vec[0] != x || vec[1] != y || vec[3] != z) {
            glUniform3f(location, x, y, z);
            assert_gl();
            vec[0] = x;
            vec[1] = y;
            vec[2] = z;
            return true;
        }
        return false;
    }

    /**
     * ベクトルを転送する
     */
    bool upload(const Vector3f &v) {
        return upload(v.x, v.y, v.z);
    }

    /**
     * float値を4つ転送する
     */
    bool upload(const float x, const float y, const float z, const float w) {
        assert(vector_length >= 4);

        if (!valid()) {
            return false;
        }

        if (vec[0] != x || vec[1] != y || vec[2] != z || vec[3] != w) {
            glUniform4f(location, x, y, z, w);
            assert_gl();
            vec[0] = x;
            vec[1] = y;
            vec[2] = z;
            vec[3] = w;
            return true;
        }
        return false;
    }

    /**
     * float値を4つ転送する
     */
    bool upload(const Vector4f &v) {
        return upload(v.x, v.y, v.z, v.w);
    }

    /**
     * 行列を直接転送する
     * mat_size == 2 : 2x2
     * mat_size == 3 : 3x3
     * mat_size == 4 : 4x4
     */
    template<uint mat_size>
    bool upload(const float *pMatrix, const uint matrix_num, const GLboolean transpose = GL_FALSE) {
        assert(vector_length >= (mat_size * mat_size * matrix_num));
        assert(pMatrix);

        if (!valid()) {
            return false;
        }

        bool upload = false;
        // 転送チェック
        for (int i = 0; i < vector_length; ++i) {
            if (vec[i] != pMatrix[i]) {
                vec[i] = pMatrix[i];
                upload = true;
            }
        }

        if (!upload) {
            // アップロードの必要がない
            return false;
        }

        // 行列の数だけ転送する
        if (mat_size == 4) {
            glUniformMatrix4fv(location, vector_length / 16, transpose, pMatrix);
        } else if (mat_size == 3) {
            glUniformMatrix3fv(location, vector_length / 9, transpose, pMatrix);
        } else if (mat_size == 2) {
            glUniformMatrix2fv(location, vector_length / 4, transpose, pMatrix);
        } else {
            assert(false);
        }
        assert_gl();

        return true;
    }

    /**
     * 44行列を転送する
     */
    bool upload(const Matrix4x4 &m, const GLboolean transpose = GL_FALSE) {
        return upload<4>((const float*) (&m), 1, transpose);
    }

    /**
     * 33行列を転送する
     */
    bool upload(const Matrix3x3 &m, const GLboolean transpose = GL_FALSE) {
        return upload<3>((const float*) (&m), 1, transpose);
    }
};

/**
 * uniform float
 */
typedef VectorUniform<float, 1> FloatUniform;

/**
 * uniform float[2]
 * uniform vec2
 */
typedef VectorUniform<float, 2> Vector2fUniform;

/**
 * uniform float[3]
 * uniform vec3
 */
typedef VectorUniform<float, 3> Vector3fUniform;

/**
 * uniform float[4]
 * uniform vec4
 */
typedef VectorUniform<float, 4> Vector4fUniofrm;

/**
 * uniform mat4
 */
typedef VectorUniform<float, 3 * 3> Matrix3Uniform;

/**
 * uniform mat4
 */
typedef VectorUniform<float, 4 * 4> Matrix4Uniform;
}
