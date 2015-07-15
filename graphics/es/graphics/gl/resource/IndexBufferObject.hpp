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
class IndexBufferObject : public GLObject {
    /**
     * 確保したインデックスバッファ
     */
    GLuint indices;

    /**
     * 1インデックスのサイズ
     */
    size_t sizeofIndex = 2;

    GLenum dataType = GL_UNSIGNED_SHORT;

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
     * @param indices_length 転送する配列の長さ
     * @param suage GL_STATIC_DRAW | GL_STREAM_DRAW | GL_DYNAMIC_DRAW
     */
    void bufferData(const uint16_t *indices, const uint indices_length, const GLenum usage) {
        this->bufferData((void *) indices, sizeof(uint16_t), indices_length, usage);
    }

    /**
     * データを転送する
     * bind()を行なってから呼び出すこと。
     * @param data 転送元のデータ
     * @param indices_length 転送するバイト数
     * @param suage GL_STATIC_DRAW | GL_STREAM_DRAW | GL_DYNAMIC_DRAW
     */
    void bufferData(const void *indices, const size_t sizeofIndexDataType, const uint indices_length, const GLenum usage) {
        this->length = indices_length;
        this->sizeofIndex = sizeofIndexDataType;
        switch (sizeofIndexDataType) {
            case 1:
                this->dataType = GL_UNSIGNED_BYTE;
                break;
            case 2:
                this->dataType = GL_UNSIGNED_SHORT;
                break;
            case 4:
                this->dataType = GL_UNSIGNED_INT;
                break;
        }
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_length * sizeofIndexDataType, (GLvoid *) indices, usage);
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
        glDrawElements(mode, length, dataType, NULL);
        assert_gl();
    }

    /**
     * 指定した範囲のレンダリングを行う
     * @param mode レンダリングモードを指定する デフォルトはGL_TRIANGLES
     * @param indices_header indices[indices_header]からレンダリングを開始する
     * @param indices_length 指定した数のインデックスバッファを描画する
     */
    void rendering(const GLenum mode, const GLsizei indicesHeaderIndex, const GLsizei indices) {
        glDrawElements(mode, indices, dataType, (GLvoid *) (sizeofIndex * indicesHeaderIndex));
    }

    virtual void dispose() {
        if (!indices) {
            return;
        }

        glDeleteBuffers(1, &indices);
        indices = 0;
        length = 0;
    }
};

/**
 * managed
 */
typedef ::std::shared_ptr<IndexBufferObject> MIndexBufferObject;
}

#endif /* INDEXBUFFEROBJECT_HPP_ */
