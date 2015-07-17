#include "MeshBuffer.h"

namespace es {

MeshBuffer::MeshBuffer(const std::shared_ptr<VertexBufferObject> &vertices, const std::shared_ptr<IndexBufferObject> &indices) :
        vertices(vertices), indices(indices) {
    assert(this->vertices);
    assert(this->indices);

    if (GPUCapacity::isSupport(GPUExtension_VertexArrayObject)) {
        glGenVertexArrays(1, &vertexArrayObject);
        assert(vertexArrayObject);

        // インデックスバッファを事前割当する
        glBindVertexArray(vertexArrayObject);
        indices->bind();
        glBindVertexArray(0);
    }
}

MeshBuffer::~MeshBuffer() {
    dispose();
}

void MeshBuffer::bind() {
    if (vertexArrayObject) {
        glBindVertexArray(vertexArrayObject);
        assert_gl();
    } else {
        vertices->bind();
        assert_gl();

        indices->bind();
        assert_gl();

        for (const Attribute &attr : attributes) {
            if (attr.location < 0) {
                continue;
            }

            // VAOをサポートしないので、手動で動作を行う
            glEnableVertexAttribArray(attr.location);
            glVertexAttribPointer(attr.location, attr.size, attr.type, attr.normalize, attr.strideBytes, Buffer::offsetBytes(nullptr, attr.offsetHeader));
            assert_gl();
        }
    }
}

void MeshBuffer::addAttribute(const MeshBuffer::Attribute &attr) {
    if (attr.location < 0) {
        // ロケーションが無効なので、シェーダーで利用されていない。
        return;
    }

    attributes.push_back(attr);

    // attributeをONにする
    if (vertexArrayObject) {
        vertices->bind();

        glEnableVertexAttribArray(attr.location);
        assert_gl();

        glVertexAttribPointer(attr.location, attr.size, attr.type, attr.normalize, attr.strideBytes, Buffer::offsetBytes(nullptr, attr.offsetHeader));
        assert_gl();
    } else {
        glEnableVertexAttribArray(attr.location);
        assert_gl();
    }

}

void MeshBuffer::rendering(const GLenum mode) {
    this->indices->rendering(mode);
}

void MeshBuffer::rendering(const GLenum mode, const GLsizei indicesHeaderIndex, const GLsizei indices) {
    this->indices->rendering(mode, indicesHeaderIndex, indices);
}

void MeshBuffer::dispose() {
    if (vertexArrayObject) {
        glDeleteVertexArrays(1, &vertexArrayObject);
        vertexArrayObject = 0;
    }
    vertices.reset();
    indices.reset();
    attributes.clear();
}

}