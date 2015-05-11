#ifndef es_graphics_gl_VERTEXATTRIBUTE_HPP_
#define es_graphics_gl_VERTEXATTRIBUTE_HPP_

#include    "es/Graphics.hpp"
#include    "es/graphics/gl/resource/ShaderProgram.h"

namespace es {

class VertexAttributeBase {
protected:
    /**
     * ロケーション
     */
    GLint location;
public:
    VertexAttributeBase() {
        location = -1;
    }

    ~VertexAttributeBase() {

    }

    /**
     * ロケーション取得する
     */
    GLint getLocation() const {
        return location;
    }

    /**
     * locationが設定済みの場合trueを返す
     * 最適化によってprogramから除かれたlocationも!valid()となる。
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
     * ロケーション設定を行う
     */
    bool setLocation(const MShaderProgram shader, const char* name) {
        assert(shader);
        assert(name);

        location = shader->getAttribLocation(name);
//        eslog("attribute location name(%s) -> %d", name, location);
        return valid();
    }

    /**
     * 有効化する
     */
    bool enable() const {
        if (!valid()) {
            return false;
        }
        glEnableVertexAttribArray(location);
        return true;
    }

    /**
     * 無効化する
     */
    bool disable() const {
        if (!valid()) {
            return false;
        }
        glDisableVertexAttribArray(location);
        return true;
    }
};

/**
 * 頂点構造体を利用した頂点属性
 *
 * @param vertex_struct 頂点構造体型
 * @param attr_size 属性数(Vec3 = 3, Vec2 = 2)
 * @param attr_type 属性タイプ(float = GL_FLOAT, u8 = GL_UNSIGNED_BYTE）
 * @param attr_normalized GL_UNSIGNED_BYTE -> GL_FLOAT変換時に0.0〜1.0に丸める場合はGL_TRUE
 * @param offset_header 頂点構造体から実際の属性までのオフセット値 {vec3, vec2}で後半にアクセスする場合はsizeof(vec3)を指定する
 */
template<typename vertex_struct, GLsizei attr_size, GLenum attr_type, GLboolean attr_normalized, GLsizei offset_header>
class VertexAttribute: public VertexAttributeBase {
public:
    VertexAttribute() {
    }

    VertexAttribute(MShaderProgram program, const char *name) {
        setLocation(program, name);
    }

    ~VertexAttribute() {
    }

    /**
     * 頂点属性の設定を行う
     */
    bool attributePointer(const GLvoid* ptr = NULL) {
        // 無効な場合は何もしない
        if (!valid()) {
            return false;
        }
        glVertexAttribPointer(location, attr_size, attr_type, attr_normalized, sizeof(vertex_struct), Buffer::offsetBytes(ptr, offset_header));
        return true;
    }
};

}

#endif /* VERTEXATTRIBUTEBASE_HPP_ */
