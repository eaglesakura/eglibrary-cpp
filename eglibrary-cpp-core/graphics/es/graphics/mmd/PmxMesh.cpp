//
// Created by @eaglesakura on 15/05/08.
//

#include "PmxMesh.h"

namespace es {

PmxMesh::PmxMesh() {

}

PmxMesh::~PmxMesh() {

}

void PmxMesh::allocVertices(const uint numVertices, const uint numExtraUV) {
    // 追加UV数は0〜4でなければならない
    assert(numExtraUV <= 4);
    // 頂点数は1以上
    assert(numVertices > 0);
    this->numExtraUV = numExtraUV;

    // 動的制御頂点を生成する
    {
        dynamicVertices.alloc(numVertices);
        metaVertices.alloc(numVertices);
        metaVertices.zeromemory();
    }

    // 固定頂点を生成する
    {
        const uint vertexSize = getStaticVertexBytes();
        eslog("PmxStaticVertex size(%d bytes) extraUV num(%d)", vertexSize, numExtraUV);
        assert(vertexSize >= sizeof(PmxStaticVertex));
        staticVertices.alloc(vertexSize * numVertices);
    }
}

void PmxMesh::allocIndices(const uint numIndex, const uint indexBytes) {
    assert(numIndex > 0);
    assert(indexBytes == 1 || indexBytes == 2 || indexBytes == 4);

    indices.alloc(numIndex * indexBytes);
}
}