#pragma once

#include    "es/OpenGL.hpp"
#include    "es/graphics/gl/resource/MeshBuffer.h"

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
 * Quad Resource
 */
class Quad : public GLObject {
    /**
     * レンダリング対象メッシュ
     */
    std::shared_ptr<MeshBuffer> mesh;

    GLenum primitiveType = GL_TRIANGLE_FAN;
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
     * 頂点情報を更新する。
     * verticesはTRIANGLE_FANで４頂点が必要。
     * nullptrの場合、デフォルトの頂点情報を利用する。
     */
    void updateVertices(const QuadVertex *vertices, const GLint posAttrLocation, const GLint uvAttrLocation);

    void setPrimitiveType(GLenum primitiveType) {
        Quad::primitiveType = primitiveType;
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
     * 開放処理
     */
    virtual void dispose();
};

typedef ::std::shared_ptr<Quad> MQuad;

}
