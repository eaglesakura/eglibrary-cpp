#include    "Quad.h"

namespace es {

/**
 * 矩形を構築する
 */
Quad::Quad() {
    this->vertices.reset(new VertexBufferObject());
    this->primitiveType = GL_TRIANGLE_FAN;
    initialize();
}

/**
 * 解放を行う。
 */
Quad::~Quad() {

}

/**
 * レンダリングの開始を宣言する
 */
void Quad::bind() {
    if (!attr.pos.valid()) {
        // 位置属性が存在しないならレンダリングできない
        return;
    }

    vertices->bind();

    attr.pos.enable();
    attr.pos.attributePointer();

    attr.coord.enable();
    attr.coord.attributePointer();
}

/**
 * 描画を行う。
 * レンダリング環境はバインド元に従う。
 */
void Quad::rendering() {
    vertices->rendering(GL_TRIANGLE_FAN, 4);
}

/**
 * 初期化を行う
 */
void Quad::initialize() {
    const float LEFT = -0.5;
    const float TOP = 0.5;
    const float RIGHT = 0.5;
    const float BOTTOM = -0.5;
    const QuadVertex g_vertices[] = {
    //
            /**
             // 位置情報
             left, top, //!< 左上
             right, top, //!< 右上
             left, bottom, //!< 左下
             right, bottom, //!< 右下

             //! UV情報
             0, 0, //!< 左上
             1, 0, //!< 右上
             0, 1, //!< 左下
             1, 1, //!< 右下
             */
            // 左上
            { LEFT, TOP, 0.0f, 0.0f, },
            // 左下
            { LEFT, BOTTOM, 0.0f, 1.0f },
            // 右下
            { RIGHT, BOTTOM, 1.0f, 1.0f },
            // 右上
            { RIGHT, TOP, 1.0f, 0.0f },
    // end
            };
    updateVertices(g_vertices);
}
/**
 * 頂点情報を更新する。
 * 4頂点を設定しなければならない。
 */
void Quad::updateVertices(const QuadVertex *vertices) {
    this->vertices->bind();
    this->vertices->bufferData<QuadVertex>(vertices, 4, GL_STATIC_DRAW);
}

/**
 * 開放処理
 */
void Quad::dispose() {
    vertices.reset();
}
}
