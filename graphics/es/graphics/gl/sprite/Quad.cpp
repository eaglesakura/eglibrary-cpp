#include    "Quad.h"

namespace es {

/**
 * 矩形を構築する
 */
Quad::Quad() {
}

/**
 * 解放を行う。
 */
Quad::~Quad() {
    dispose();
}

void Quad::updateVertices(const QuadVertex *vertices, const GLint posAttrLocation, const GLint uvAttrLocation) {
    if (mesh) {
        // 古いメッシュを解放する
        mesh.reset();
    }

    if (posAttrLocation < 0) {
        // 位置属性が無いのでレンダリングできない。
        return;
    }

    const float LEFT = -0.5;
    const float TOP = 0.5;
    const float RIGHT = 0.5;
    const float BOTTOM = -0.5;
    const QuadVertex defVertices[] = {
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
            {LEFT,  TOP,    0.0f, 0.0f,},
            // 左下
            {LEFT,  BOTTOM, 0.0f, 1.0f},
            // 右下
            {RIGHT, BOTTOM, 1.0f, 1.0f},
            // 右上
            {RIGHT, TOP,    1.0f, 0.0f},
            // end
    };

    const QuadVertex *usingVertices = vertices ? vertices : defVertices;

    std::vector<uint8_t> indices = {0, 1, 2, 3};
    assert(indices.size() == 4);
    std::shared_ptr<IndexBufferObject> ibo = IndexBufferObject::newInstance(indices);
    std::shared_ptr<VertexBufferObject> vbo(new VertexBufferObject());

    vbo->bind();
    vbo->bufferData<QuadVertex>(usingVertices, 4, GL_STATIC_DRAW);

    mesh.reset(new MeshBuffer(vbo, ibo));
    mesh->bind();
    {
        MeshBuffer::Attribute attr;
        attr.location = posAttrLocation;
        attr.normalize = false;
        attr.offsetHeader = 0;
        attr.size = 2;
        attr.type = GL_FLOAT;
        attr.strideBytes = sizeof(QuadVertex);
        mesh->addAttribute(attr);
    }
    {
        MeshBuffer::Attribute attr;
        attr.location = uvAttrLocation;
        attr.normalize = false;
        attr.offsetHeader = sizeof(vec2);
        attr.size = 2;
        attr.type = GL_FLOAT;
        attr.strideBytes = sizeof(QuadVertex);
        mesh->addAttribute(attr);
    }

    glFinish();
}

/**
 * レンダリングの開始を宣言する
 */
void Quad::bind() {
    if (!mesh) {
        return;
    }
    mesh->bind();
}

/**
 * 描画を行う。
 * レンダリング環境はバインド元に従う。
 */
void Quad::rendering() {
    if (!mesh) {
        return;
    }
//    vertices->rendering(GL_TRIANGLE_FAN, 4);
    mesh->getVertices()->rendering(GL_TRIANGLE_FAN, 4);
//    mesh->rendering(primitiveType);
}


/**
 * 開放処理
 */
void Quad::dispose() {
    mesh.reset();
}

}
