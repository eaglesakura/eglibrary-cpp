#ifndef es_graphics_gl_VERTEXBUFFEROBJECT_HPP_
#define es_graphics_gl_VERTEXBUFFEROBJECT_HPP_

#include    "es/Graphics.hpp"
#include    "es/graphics/gl/GLObject.h"

namespace es {

/**
 * 頂点バッファオブジェクトを管理する
 *
 * 生成は内部的に行うが、廃棄はVRAMのgcに任せる
 * Context間を移動する際は必ずunbind()を行い、stateから外すこと。
 */
class VertexBufferObject: public GLObject {
    /**
     * 確保したVBO
     */
    GLuint vertices;

public:
    VertexBufferObject() {
        glGenBuffers(1, &vertices);
    }

    virtual ~VertexBufferObject() {
        dispose();
    }

    /**
     * バッファにデータを転送する。
     * 必ずbind()済みの状態で呼び出すこと
     * @param source 転送元のデータポインタ
     * @param sizeof_vertex ソースの頂点サイズ
     * @param length ソースの頂点数
     * @param suage GL_STATIC_DRAW | GL_STREAM_DRAW | GL_DYNAMIC_DRAW
     */
    void bufferData(const void *vertices, const uint sizeof_vertex, const uint vertices_length, const GLenum usage) {
        glBufferData(GL_ARRAY_BUFFER, sizeof_vertex * vertices_length, (GLvoid*) vertices, usage);
        assert_gl();
    }

    /**
     * バッファにデータを転送する。
     * 必ずbind()済みの状態で呼び出すこと
     * @param source 転送元のデータポインタ
     * @param sizeof_vertex ソースの頂点サイズ
     * @param length ソースの頂点数
     * @param suage GL_STATIC_DRAW | GL_STREAM_DRAW | GL_DYNAMIC_DRAW
     */
    template<typename VertexType>
    void bufferData(const VertexType *vertices, const uint vertices_length, const GLenum usage) {
        glBufferData(GL_ARRAY_BUFFER, sizeof(VertexType) * vertices_length, (GLvoid*) vertices, usage);
        assert_gl();
    }

    /**
     * GLのステートへと関連付ける
     */
    void bind() {
        glBindBuffer(GL_ARRAY_BUFFER, vertices);
    }

    /**
     * レンダリングを行う
     *
     * @param mode GL_TRIANGLE_FAN等
     * @param vertices_length 利用する頂点数
     */
    void rendering(GLenum mode, GLuint vertices_length) {
        glDrawArrays(mode, 0, vertices_length);
    }

    /**
     *
     */
    void dispose() {
        if (!vertices) {
            return;
        }

        glDeleteBuffers(1, &vertices);
        vertices = 0;
    }
};

typedef std_shared_ptr<VertexBufferObject> MVertexBufferObject;

}

#endif /* VERTEXBUFFEROBJECT_HPP_ */
