//
// Created by @eaglesakura on 15/05/08.
//

#include <es/graphics/mmd/PmxMesh.h>
#include <es/util/StringUtil.h>
#include "PmxFileLoader.h"

namespace es {

PmxFileLoader::PmxFileLoader() {
    
}

PmxFileLoader::~PmxFileLoader() {
    
}

MPmxFile PmxFileLoader::loadPmx(const unsafe_array<uint8_t> buffer) {
    MmdBufferDataLoader loader(buffer);
    
    MPmxFile result(new PmxFile());
    if (!loadPmxHeader(&loader, result)) {
        return MPmxFile();
    }
    
    result->figure.reset(new PmxFigure());
    if (!loadPmxVertices(&loader, result)) {
        return MPmxFile();
    }
    
    if (!loadPmxFaces(&loader, result)) {
        return MPmxFile();
    }
    
    if (!loadPmxMaterials(&loader, result)) {
        return MPmxFile();
    }

    if (!loadPmxBones(&loader, result)) {
        return MPmxFile();
    }

    return result;
}

bool PmxFileLoader::loadPmxHeader(MmdBufferDataLoader *loader, MPmxFile result) {
    
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
        eslog("PMX version(%.1f)", result->header->version);
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

bool PmxFileLoader::loadPmxVertices(MmdBufferDataLoader *loader, MPmxFile result) {
    
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
    
    vec3 maxPosition(-999999, -999999, -999999);
    vec3 minPosition(999999, 999999, 999999);
    
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
        minPosition.x = std::min(minPosition.x, dynamicVertex->pos.x);
        minPosition.y = std::min(minPosition.y, dynamicVertex->pos.y);
        minPosition.z = std::min(minPosition.z, dynamicVertex->pos.z);
        
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
    
    mesh->setMinPosition(minPosition);
    mesh->setMaxPosition(maxPosition);
    result->figure->setMesh(mesh);
    
    return true;
}

bool PmxFileLoader::loadPmxFaces(MmdBufferDataLoader *loader, MPmxFile result) {
    
    const uint numIndices = loader->loadInt32();
    eslog("numIndices index(%d) face(%d)", numIndices, numIndices / 3);
    assert(numIndices > 0 && numIndices % 3 == 0);
    
    // インデックス情報を確保する
    result->figure->getMesh()->allocIndices(numIndices, result->header->vertexIndexSize);
    uint8_t *pointer = result->figure->getMesh()->getIndicesPointer();
    loader->loadBuffer(pointer, numIndices * result->header->vertexIndexSize);

    return true;
}

bool PmxFileLoader::loadPmxMaterials(MmdBufferDataLoader *loader, MPmxFile result) {
    
    const uint numTextures = loader->loadInt32();
    eslog("numTextures(%d)", numTextures);
    safe_array<MPmxTexture> figureTextures;
    safe_array<MPmxTexture> mmdToonTextures;    // デフォルトToonテクスチャ
    figureTextures.reserve(numTextures);
    mmdToonTextures.reserve(numTextures);
    
    for (int i = 0; i < numTextures; ++i) {
        const std::string path = loader->loadTextBuffer();
        figureTextures[i].reset(new PmxTexture(path));
        eslog("texture[%d] path(%s)", i, path.c_str());
    }
    for (int i = 0; i < mmdToonTextures.length; ++i) {
        figureTextures[i].reset(new PmxTexture(StringUtils::format("%02d.bmp", i + 1)));
    }
    
    const uint numMaterials = loader->loadInt32();
    eslog("numMaterials(%d)", numMaterials);
    
    const uint textureIndexSize = result->header->textureIndexSize;
    
    for (int i = 0; i < numMaterials; ++i) {
        MPmxMaterial material(new PmxMaterial());
        material->setName(loader->loadTextBuffer());
        material->setNameEng(loader->loadTextBuffer());
        eslog("material[%d] name(%s)", i, material->getName().c_str());
        
        material->setDiffuse(loader->loadRGBA());
        material->setSpecular(loader->loadRGB());
        material->setShininess(loader->loadFloat());
        material->setAmbient(loader->loadRGB());
        material->setRenderFlags(loader->loadByte());
        material->setEdgeColor(loader->loadRGBA());
        material->setEdgeSize(loader->loadFloat());
        
        {
            int diffuseIndex = loader->loadIntN(textureIndexSize);
            int sphereIndex = loader->loadIntN(textureIndexSize);
            eslog("    diffuse(%d) sphere(%d)", diffuseIndex, sphereIndex);
            if (diffuseIndex >= 0) {
                material->setDiffuseTexture(figureTextures[diffuseIndex]);
            }
            if (sphereIndex >= 0) {
                material->setSphereTexture(mmdToonTextures[sphereIndex]);
            }
        }
        material->setSphereMode((PmxMaterial::SphereMode) loader->loadByte());
        material->setSharedSphereToon(loader->loadByte() != 0);
        
        {
            int toonTextureIndex = loader->loadIntN(material->isSharedSphereToon() ? 1 : textureIndexSize);
            eslog("    toonTextureIndex(%d) shared(%s)", toonTextureIndex, material->isSharedSphereToon() ? "true" : "false");
            if (toonTextureIndex >= 0) {
                if (material->isSharedSphereToon()) {
                    // 共有toon
                    material->setToonTexture(mmdToonTextures[toonTextureIndex]);
                } else {
                    // 固有Toonテクスチャ
                    material->setToonTexture(figureTextures[toonTextureIndex]);
                }
            }
        }
        material->setMemo(loader->loadTextBuffer());
        material->setIndicesCount(loader->loadInt32());
        
        eslog("    memo(%s)", material->getMemo().c_str());
        eslog("    numIndices(%d) faces(%d)", material->getIndicesCount(), material->getIndicesCount() / 3);
        
        result->figure->addMaterial(material);
    }
    
    
    return true;
}

bool PmxFileLoader::loadPmxBones(MmdBufferDataLoader *loader, MPmxFile result) {

    const uint numBones = loader->loadInt32();

    const uint boneIndexSize = result->header->boneIndexSize;
    eslog("numBones(%d) boneIndexSize(%d)", numBones, boneIndexSize);

    std::vector<MPmxBone> bones;
    for (int i = 0; i < numBones; ++i) {
        MPmxBone bone(new PmxBone());

        bone->setIndex(i);
        bone->setName(loader->loadTextBuffer());
        bone->setNameEng(loader->loadTextBuffer());
        eslog("bone[%d] name(%s)", i, bone->getName().c_str());

        bone->setPosition(loader->loadVector3());
        bone->setParentBoneIndex(loader->loadIntN(boneIndexSize));
        eslog("    bone pos(%.2f, %.2f, %.2f)", bone->getPosition().x, bone->getPosition().y, bone->getPosition().z);
        eslog("    bone parent(%d)", bone->getParentBoneIndex());
        bone->setDeformationLevel(loader->loadInt32());
        bone->setFlags(0, loader->loadByte());
        bone->setFlags(1, loader->loadByte());

        if (bone->hasFlag(PmxBone::Flag::ConnectionDisplayMethod)) {
            bone->setConnectedBoneIndex(loader->loadIntN(boneIndexSize));
            eslog("    bone connect to(%d)", bone->getConnectedBoneIndex());
        } else {
            bone->setPositionOffset(loader->loadVector3());
            eslog("    bone offset(%.2f, %.2f, %.2f)", bone->getPositionOffset().x, bone->getPositionOffset().y, bone->getPositionOffset().z);
        }

        if (bone->hasFlag(PmxBone::Flag::GiveRotation) || bone->hasFlag(PmxBone::Flag::GiveTranslation)) {
            bone->setProvidedParentBoneIndex(loader->loadIntN(boneIndexSize));
            bone->setProvidedRatio(loader->loadFloat());
        }

        if (bone->hasFlag(PmxBone::Flag::AxisFixed)) {
            bone->setAxisDirectionVector(loader->loadVector3());
        }

        if (bone->hasFlag(PmxBone::Flag::LocalAxis)) {
            bone->setDimentionXDirectionVector(loader->loadVector3());
            bone->setDimentionZDirectionVector(loader->loadVector3());
        }
        if (bone->hasFlag(PmxBone::Flag::ExternalParentTransform)) {
            bone->setKeyValue(loader->loadInt32());
        }

        if (bone->hasFlag(PmxBone::Flag::IkEnable)) {
            bone->setIkTargetBoneIndex(loader->loadIntN(boneIndexSize));
            bone->setIkLoopCount(loader->loadInt32());
            bone->setIkLimitedRadian(loader->loadFloat());
            uint ikLinkCount = loader->loadInt32();
            eslog("    ikLinkCount(%d)", ikLinkCount);

            bone->allocIkLinkList(ikLinkCount);
            for (int k = 0; k < ikLinkCount; ++k) {
                PmdIkLink *pIkLink = bone->getIkLink(k);
                pIkLink->linkBoneIndex = loader->loadIntN(boneIndexSize);
                pIkLink->rotateLimited = loader->loadByte() != 0;
                eslog("    ikLink[%d] limited[%s]", k, pIkLink->rotateLimited ? "true" : "false");
                if (pIkLink->rotateLimited) {
                    pIkLink->minRadian = loader->loadVector3();
                    pIkLink->maxRadian = loader->loadVector3();
                }
            }
        }

        bones.push_back(bone);
    }

    MPmxBoneController boneController(new PmxBoneController());
    boneController->initialize(bones);

    result->figure->setBoneController(boneController);

    return true;
}
}