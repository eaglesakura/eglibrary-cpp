#pragma once

#include    "es/eglibrary.hpp"
#include    "es/OpenGL.hpp"
#include    "VertexBufferObject.hpp"
#include    "IndexBufferObject.hpp"

namespace es {

/**
 * メッシュ操作を行う
 *
 * 内部はVBO/IBOのセットを持つ。
 * 実装をシンプルにするため、VBO/IBOはそれぞれ１つずつ管理とする。
 * １種類のシェーダーに対して１つ設定する。
 */
class MeshBuffer : public GLObject {
public:
    /**
     * 設定する属性情報
     */
    struct Attribute {
        /**
         * 属性数(Vec3 = 3, Vec2 = 2)
         */
        uint8_t size;

        /**
         * 頂点の隙間量
         * sizeof(VertexStruct)
         */
        uint8_t strideBytes;

        /**
         * GL_UNSIGNED_BYTE -> GL_FLOAT変換時に0.0〜1.0に丸める場合はGL_TRUE
         */
        bool normalize;

        /**
         * 頂点構造体から実際の属性までのオフセット値 {vec3, vec2}で後半にアクセスする場合はsizeof(vec3)を指定する
         */
        uint8_t offsetHeader;

        /**
         * attribute loc
         */
        int8_t location;

        /**
         * 属性タイプ(float = GL_FLOAT, byte = GL_UNSIGNED_BYTE）
         */
        GLenum type;
    };

    MeshBuffer(const std::shared_ptr<VertexBufferObject> &vertices, const std::shared_ptr<IndexBufferObject> &indices);

    virtual ~MeshBuffer();

    virtual void bind();

    /**
     * レンダリング用の属性情報を追加する
     */
    virtual void addAttribute(const Attribute &attr);

    /**
     * レンダリングを行う
     * @param mode レンダリングモードを指定する デフォルトはGL_TRIANGLES
     */
    void rendering(const GLenum mode);

    /**
     * 指定した範囲のレンダリングを行う
     * @param mode レンダリングモードを指定する デフォルトはGL_TRIANGLES
     * @param indices_header indices[indices_header]からレンダリングを開始する
     * @param indices_length 指定した数のインデックスバッファを描画する
     */
    void rendering(const GLenum mode, const GLsizei indicesHeaderIndex, const GLsizei indices);


    virtual void dispose();


    const std::shared_ptr<VertexBufferObject> &getVertices() const;

    const std::shared_ptr<IndexBufferObject> &getIndices() const;

private:
    /**
     * サポートしている場合はVAOを利用する。
     *
     * しない場合は別な方法を利用する。
     */
    GLuint vertexArrayObject = 0;

    std::shared_ptr<VertexBufferObject> vertices;
    std::shared_ptr<IndexBufferObject> indices;

    /**
     * 利用する属性一覧
     */
    std::vector<Attribute> attributes;
};

}

