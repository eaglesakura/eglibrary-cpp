#ifndef es_graphics_gl_INDEXBUFFEROBJECT_HPP_
#define es_graphics_gl_INDEXBUFFEROBJECT_HPP_

#include    "es/Graphics.hpp"
#include    "es/graphics/gl/GLObject.h"

namespace es {

/**
 * インデックスバッファを管理する
 *
 * 生成は内部で行うが、廃棄はVRAMクラスのgcに任せる。
 * Context間を移動する場合は必ずunbindを行い、ステートの残骸を残さないよう注意すること
 */
class IndexBufferObject: public GLObject {
    /**
     * 確保したインデックスバッファ
     */
    GLuint indices;

    /**
     * インデックスバッファの配列長
     */
    uint length;

public:
    IndexBufferObject() {
        glGenBuffers(1, &indices);
        this->length = 0;
    }

    virtual ~IndexBufferObject() {
        dispose();
    }

    /**
     * GLへ関連付ける
     */
    void bind() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);
    }

    /**
     * データを転送する
     * bind()を行なってから呼び出すこと。
     * @param data 転送元のデータ
     * @param size 転送するバイト数
     * @param suage GL_STATIC_DRAW | GL_STREAM_DRAW | GL_DYNAMIC_DRAW
     */
    void bufferData(const uint16_t *indices, const uint indices_length, const GLenum usage) {
        this->length = indices_length;
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_length * sizeof(uint16_t), (GLvoid*) indices, usage);
        assert_gl();
    }

    /**
     * 配列の長さを取得する
     */
    uint getLength() {
        return length;
    }

    /**
     * レンダリングを行う
     * @param mode レンダリングモードを指定する デフォルトはGL_TRIANGLES
     */
    void rendering(const GLenum mode) {
        glDrawElements(mode, length, GL_UNSIGNED_SHORT, NULL);
        assert_gl();
    }

    /**
     * 指定した範囲のレンダリングを行う
     * @param mode レンダリングモードを指定する デフォルトはGL_TRIANGLES
     * @param indices_header indices[indices_header]からレンダリングを開始する
     * @param indices_length 指定した数のインデックスバッファを描画する
     */
    void rendering(const GLenum mode, const GLsizei indicesHeaderIndex, const GLsizei indices) {
        glDrawElements(mode, indices, GL_UNSIGNED_SHORT, (GLvoid*) (sizeof(uint16_t) * indicesHeaderIndex));
    }

    virtual void dispose() {
        if (!indices) {
            return;
        }

        glDeleteBuffers(1, &indices);
        indices = 0;
    }
};

/**
 * managed
 */
typedef std_shared_ptr<IndexBufferObject> MIndexBufferObject;
}

#endif /* INDEXBUFFEROBJECT_HPP_ */
