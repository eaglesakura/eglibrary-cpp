//
// Created by @eaglesakura on 15/05/08.
//

#include <es/graphics/mmd/PmxMesh.h>
#include "MmdFileLoader.h"

namespace es {

MmdFileLoader::MmdFileLoader() {

}

MmdFileLoader::~MmdFileLoader() {

}

MPmxFile MmdFileLoader::loadPmx(const unsafe_array<uint8_t> buffer) {
    MmdBufferDataLoader loader(buffer);

    MPmxFile result(new PmxFile());
    if (!loadPmxHeader(&loader, result)) {
        return MPmxFile();
    }

    result->figure.reset(new PmxFigure());
    if (!loadPmxVertices(&loader, result)) {
        return MPmxFile();
    }

    return result;
}

bool MmdFileLoader::loadPmxHeader(MmdBufferDataLoader *loader, MPmxFile result) {

    // load headers
    {
        const std::string magic = loader->loadFileMagic(3);
        eslog("magic(%s)", magic.c_str());
        if (magic != "PMX") {
            return false;
        }
        loader->loadByte();  // skip 1byte
    }

    // load fileversion
    result->header->version = loader->loadFloat();
    if (result->header->version > 2) {
        eslog("PMX unknown version(%f)", result->header->version);
        return false;
    } else {
        eslog("PMX version(%f)", result->header->version);
        const uint numHeaderDatas = loader->loadByte();
        assert(numHeaderDatas == 8); // PMX 2.0 = 8

        result->header->encodeType = loader->loadByte();
        result->header->addUVCount = loader->loadByte();
        result->header->vertexIndexSize = loader->loadByte();
        result->header->textureIndexSize = loader->loadByte();
        result->header->materialIndexSize = loader->loadByte();
        result->header->boneIndexSize = loader->loadByte();
        result->header->morphIndexSize = loader->loadByte();
        result->header->rigidIndexSize = loader->loadByte();

        eslog("encodeType(%d)", (int) result->header->encodeType);
        eslog("addUVCount(%d)", (int) result->header->addUVCount);
        eslog("vertexIndexSize(%d)", (int) result->header->vertexIndexSize);
        eslog("textureIndexSize(%d)", (int) result->header->textureIndexSize);
        eslog("materialIndexSize(%d)", (int) result->header->materialIndexSize);
        eslog("boneIndexSize(%d)", (int) result->header->boneIndexSize);
        eslog("morphIndexSize(%d)", (int) result->header->morphIndexSize);
        eslog("rigidIndexSize(%d)", (int) result->header->rigidIndexSize);

        loader->setTextEncodeType(result->header->encodeType);
    }

    // load modelinfo
    result->info->name = loader->loadTextBuffer();
    result->info->nameEng = loader->loadTextBuffer();
    result->info->comment = loader->loadTextBuffer();
    result->info->commentEng = loader->loadTextBuffer();
    // ログで確認
    // オーバーフローを防ぐためにコメントはログ出ししない
    eslog("name(%s)", result->info->name.c_str());
    eslog("nameEng(%s)", result->info->nameEng.c_str());
//    eslog("comment(%s)", result->info->comment.c_str());
//    eslog("commentEng(%s)", result->info->commentEng.c_str());

    return true;
}

bool MmdFileLoader::loadPmxVertices(MmdBufferDataLoader *loader, MPmxFile result) {

    uint numVertices = loader->loadInt32();
    eslog("PMX numVertices(%d)", numVertices);
    assert(numVertices > 0);
    MPmxMesh mesh(new PmxMesh());

    // 頂点を生成する
    const auto addUVCount = result->header->addUVCount;
    const auto boneIndexSize = result->header->boneIndexSize;

    // デフォルトの頂点読み込み関数
    auto boneIndexLoadFunc = [loader, boneIndexSize](int16_t *result, int numBones) -> void {
        if (boneIndexSize == 2) {
            loader->loadBuffer(result, sizeof(uint16_t) * numBones);
        } else if (boneIndexSize == 1) {
            for (int i = 0; i < numBones; ++i) {
                *result = loader->loadByte();
            }
        } else {
            for (int i = 0; i < numBones; ++i) {
                *result = loader->loadInt32();
            }
        }
    };

    mesh->allocVertices(numVertices, addUVCount);

    PmxDynamicVertex *dynamicVertex = mesh->getDynamicVerticesPointer();
    PmxMetaVertex *metaVertex = mesh->getMetaVerticesPointer();
    uint8_t *rawStaticVertex = mesh->getStaticVerticesPointer();
    const uint32_t staticVertexBytes = mesh->getStaticVertexBytes();

    Vector3f maxPosition(-999999, -999999, -999999);
    Vector3f minPosition(999999, 999999, 999999);

    for (int i = 0; i < numVertices; ++i) {
        PmxStaticVertex *staticVertex = (PmxStaticVertex *) rawStaticVertex;

        // 各種データを読み込む
        loader->loadBuffer(&dynamicVertex->pos, sizeof(Vector3f));
        loader->loadBuffer(&dynamicVertex->normal, sizeof(Vector3f));
        loader->loadBuffer(&staticVertex->uv, sizeof(Vector2f));
        // 拡張UVを読み込む
        for (int k = 0; k < addUVCount; ++k) {
            loader->loadBuffer(&(staticVertex->extraUv[k]), sizeof(Vector4f));
        }

        // ボーン情報を読み込む
        metaVertex->boneType = loader->loadByte();
        if (i % 1000 == 0) {
            eslog("index(%d) bone(%d) pos(%.3f, %.3f, %.3f)", i, metaVertex->boneType, dynamicVertex->pos.x, dynamicVertex->pos.y, dynamicVertex->pos.z);
        }

        switch (metaVertex->boneType) {
            case PmxMetaVertex::BDEF1:
                boneIndexLoadFunc(metaVertex->boneIndices, 1);
                metaVertex->boneWeights[0] = 1.0f;
                break;
            case PmxMetaVertex::BDEF2:
                boneIndexLoadFunc(metaVertex->boneIndices, 2);
                metaVertex->boneWeights[0] = loader->loadFloat();
                metaVertex->boneWeights[1] = 1.0f - metaVertex->boneWeights[0];
                break;
            case PmxMetaVertex::BDEF4:
                boneIndexLoadFunc(metaVertex->boneIndices, 4);
                loader->loadBuffer(metaVertex->boneWeights, sizeof(float) * 4);
//                metaVertex->boneWeights[3] = 1.0f - (metaVertex->boneWeights[0] + metaVertex->boneWeights[1] + metaVertex->boneWeights[2]);
                break;
            case PmxMetaVertex::SDEF:
                boneIndexLoadFunc(metaVertex->boneIndices, 2);
                metaVertex->boneWeights[0] = loader->loadFloat();
                metaVertex->boneWeights[1] = 1.0f - metaVertex->boneWeights[0];

                loader->loadBuffer(&metaVertex->boneSdef.c, sizeof(Vector3f));
                loader->loadBuffer(&metaVertex->boneSdef.r0, sizeof(Vector3f));
                loader->loadBuffer(&metaVertex->boneSdef.r1, sizeof(Vector3f));
                break;
#ifdef DEBUG
            default:
                assert(false);
#endif
        }
        loader->loadBuffer(&staticVertex->edgeMagnification, sizeof(float));

        // 最小・最大位置を求める
        maxPosition.x = std::max(maxPosition.x, dynamicVertex->pos.x);
        maxPosition.y = std::max(maxPosition.y, dynamicVertex->pos.y);
        maxPosition.z = std::max(maxPosition.z, dynamicVertex->pos.z);
        minPosition.x = std::min(maxPosition.x, dynamicVertex->pos.x);
        minPosition.y = std::min(maxPosition.y, dynamicVertex->pos.y);
        minPosition.z = std::min(maxPosition.z, dynamicVertex->pos.z);

        // 次の頂点へ進める
        ++dynamicVertex;
        ++metaVertex;
        rawStaticVertex += staticVertexBytes;
    }

    eslog("min(%.3f, %.3f, %.3f) max(%.3f, %.3f, %.3f) box WHD(%.3f, %.3f, %.3f)",
          minPosition.x, minPosition.y, minPosition.z,
          maxPosition.x, maxPosition.y, maxPosition.z,
          maxPosition.x - minPosition.x, maxPosition.y - minPosition.y, maxPosition.z - minPosition.z
    );


    return true;
}
}