#pragma once

#include    "es/OpenGL.hpp"

namespace es {

/**
 * 四角形構築用の頂点構造体
 */
struct QuadVertex {
    float x;
    float y;

    float u;
    float v;
};

/**
 * 位置属性
 */
typedef VertexAttribute<QuadVertex, 2, GL_FLOAT, GL_FALSE, 0> QuadPositionAttribute;

/**
 * UV属性
 */
typedef VertexAttribute<QuadVertex, 2, GL_FLOAT, GL_FALSE, (GLsizei) sizeof(Vector2f)> QuadCoordAttribute;

/**
 * Quad Resource
 */
class Quad: public GLObject {
    /**
     * Vertices
     */
    MVertexBufferObject vertices;

    struct {
        /**
         * 位置情報
         */
        QuadPositionAttribute pos;

        /**
         * UV情報
         */
        QuadCoordAttribute coord;
    } attr;
    /**
     * 初期化を行う
     */
    void initialize();

    /**
     * レンダリングモード
     */
    GLenum primitiveType;
public:
    /**
     * 矩形を構築する
     */
    Quad();

    /**
     * 解放を行う。
     */
    virtual ~Quad();

    /**
     *
     */
    virtual void setPositionAttribute(const QuadPositionAttribute &attr) {
        this->attr.pos = attr;
    }

    /**
     *
     */
    virtual void setCoordAttribute(const QuadCoordAttribute &attr) {
        this->attr.coord = attr;
    }

    /**
     * 描画するプリミティブの種類を変更する
     */
    virtual void setPrimitiveType(const GLenum type) {
        this->primitiveType = type;
    }

    /**
     * レンダリングの開始を宣言する
     */
    virtual void bind();

    /**
     * 描画を行う。
     * レンダリング環境はバインド元に従う。
     */
    virtual void rendering();

    /**
     * 頂点情報を更新する。
     * 4頂点を設定しなければならない。
     */
    virtual void updateVertices(const QuadVertex *vertices);

    /**
     * 開放処理
     */
    virtual void dispose();
};

typedef ::std::shared_ptr<Quad> MQuad;

}
