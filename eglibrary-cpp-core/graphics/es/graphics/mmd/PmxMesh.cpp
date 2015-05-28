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
        skinVertices.alloc(numVertices);
        metaVertices.alloc(numVertices);
        skinVertices.zeromemory();
        metaVertices.zeromemory();
    }

    // 固定頂点を生成する
    {
        const uint vertexSize = getMeshVertexBytes();
        eslog("PmxMeshVertex size(%d bytes) extraUV num(%d)", vertexSize, numExtraUV);
        assert(vertexSize >= sizeof(PmxMeshVertex));
        meshVertices.alloc(vertexSize * numVertices);
    }
}

void PmxMesh::allocIndices(const uint numIndex, const uint indexBytes) {
    assert(numIndex > 0);
    assert(indexBytes == 1 || indexBytes == 2 || indexBytes == 4);

    this->indexBytes = indexBytes;
    indices.alloc(numIndex * indexBytes);
}

void PmxMesh::setBoneModifyGroupIndices(const std::vector<uint> &bdef1, const std::vector<uint> &bdef2, const std::vector<uint> &bdef3, const std::vector<uint> &bdef4, const std::vector<uint> &sdef) {
    if (!bdef1.empty()) {
        modifyTypeGroups.bdef1.alloc(bdef1.size());
        modifyTypeGroups.bdef1.memcopyFrom(&bdef1[0], bdef1.size());
    }
    if (!bdef2.empty()) {
        modifyTypeGroups.bdef2.alloc(bdef2.size());
        modifyTypeGroups.bdef2.memcopyFrom(&bdef2[0], bdef2.size());
    }
    if (!bdef3.empty()) {
        modifyTypeGroups.bdef3.alloc(bdef3.size());
        modifyTypeGroups.bdef3.memcopyFrom(&bdef3[0], bdef3.size());
    }
    if (!bdef4.empty()) {
        modifyTypeGroups.bdef4.alloc(bdef4.size());
        modifyTypeGroups.bdef4.memcopyFrom(&bdef4[0], bdef4.size());
    }
    if (!sdef.empty()) {
        modifyTypeGroups.sdef.alloc(sdef.size());
        modifyTypeGroups.sdef.memcopyFrom(&sdef[0], sdef.size());
    }
}
}