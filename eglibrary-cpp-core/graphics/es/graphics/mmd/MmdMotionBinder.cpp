#include "MmdMotionBinder.h"

namespace es {


MmdMotionBinder::MmdMotionBinder() {
    
}

void MmdMotionBinder::bind(const MPmxBoneData &boneData, const MVmdMotionData &motionData) {
    assert(boneData);
    assert(motionData);
    
    eslog("bind MotionBase(%s)", motionData->getModelName().c_str());
    
    this->boneData = boneData;
    this->motionData = motionData;
    
    // 必要な容量を確保
    this->binds.refresh(boneData->getBoneCount());
    this->boneMatrixTable.refresh(boneData->getBoneCount());
    this->vertexMatrixTable.refresh(boneData->getBoneCount());
    this->localMatrixTable.refresh(boneData->getBoneCount());
    
    for (uint i = 0; i < binds.length; ++i) {
        // ボーンの基本データを更新する
        BoneBind *pBind = binds.ptr + i;
        
        pBind->pBone = boneData->getBone(i);
        assert(pBind->pBone);
        
        pBind->boneMotion = motionData->findBoneController(pBind->pBone->self->getName(), false);
        eslog("    PmxBone[%d] name(%s) -> %s", i, pBind->pBone->self->getName().c_str(), (bool) pBind->boneMotion ? "Link" : "Skip");

        pBind->pLocalMatrix = localMatrixTable.ptr + i;
        pBind->pGlobalMatrix = boneMatrixTable.ptr + i;
        pBind->pVertexMatrix = vertexMatrixTable.ptr + i;
    }
}


void MmdMotionBinder::setMotionFrame(float currentMotionFrame) {
    if (motionFillType == Loop) {
        while (currentMotionFrame < 0) {
            currentMotionFrame += motionData->getAllFrames();
        }
        
        float allFrames = loopRange.end;
        if (allFrames <= 0) {
            allFrames = motionData->getAllFrames();
        }
        bool overFrame = currentMotionFrame > allFrames;
        
        while (currentMotionFrame > allFrames) {
            currentMotionFrame -= allFrames;
        }
        
        // ループ中で、かつループレンジ指定がある場合
        if (overFrame && loopRange.start > 0) {
            while (currentMotionFrame < loopRange.start) {
                currentMotionFrame += loopRange.start;
            }

//            currentMotionFrame = allFrames;
        }
        
        this->currentMotionFrame = currentMotionFrame;
    } else {
        // fill
        this->currentMotionFrame = es::minmax(0.0f, (float) motionData->getAllFrames(), currentMotionFrame);
    }
}

/**
 * モーションを計算する
 *
 * FIXME リンク影響ボーンの設定を行う
 */
void MmdMotionBinder::calcMotion() {
//    eslog("MotionFrame(%.1f)", currentMotionFrame);
    unsafe_array<BoneBind> iterator = binds.iterator();
    while (iterator) {
        const uint matrixIndex = iterator->pBone->self->getIndex();

        if (iterator->pBone->self->hasProvidedParentBone()) {
            // TODO ProvidedParentBoneIndexが後に来る可能性があるので、アルゴリズムの改良が必要
//            assert(iterator->pBone->self->getIndex() > iterator->pBone->self->getProvidedParentBoneIndex());
        }
        
        if (iterator->boneMotion) {
            iterator->boneMotion->getKey(currentMotionFrame, &iterator->motionTranslate, &iterator->motionRotate);
            mat4 motion =
                    glm::translate(iterator->motionTranslate) *
                    glm::toMat4(iterator->motionRotate);
            
            // 親の有無で分ける
            if (iterator->pBone->parent) {
                // 親が存在するなら、親の結果に従う
                const uint parentIndex = iterator->pBone->parent->getIndex();

                localMatrixTable.ptr[matrixIndex] =
                        glm::translate(iterator->pBone->offset) *
                        motion;
                
                boneMatrixTable.ptr[matrixIndex] =
                        boneMatrixTable.ptr[parentIndex] *
                        localMatrixTable.ptr[matrixIndex];
                
                vertexMatrixTable.ptr[matrixIndex] =
                        boneMatrixTable.ptr[matrixIndex] *
                        iterator->pBone->invert;
            } else {
                // 親が存在しないならモーションを直接適用する
                boneMatrixTable.ptr[matrixIndex] = motion;
                vertexMatrixTable.ptr[matrixIndex] = boneMatrixTable.ptr[matrixIndex] * iterator->pBone->invert;
            }
        } else {
            // 親の有無で分ける
            if (iterator->pBone->parent) {
                // 親が存在するなら、親の結果を引き継ぐ
                const uint parentIndex = iterator->pBone->parent->getIndex();

                localMatrixTable.ptr[matrixIndex] = glm::translate(iterator->pBone->offset);

                boneMatrixTable.ptr[matrixIndex] =
                        boneMatrixTable.ptr[parentIndex] *
                        localMatrixTable.ptr[matrixIndex];

                vertexMatrixTable.ptr[matrixIndex] =
                        boneMatrixTable.ptr[matrixIndex] *
                        iterator->pBone->invert;
            } else {

//                localMatrixTable.ptr[matrixIndex] = glm::translate(iterator->pBone->offset);
//
//                boneMatrixTable.ptr[matrixIndex] =
//                        localMatrixTable.ptr[matrixIndex];
//                vertexMatrixTable.ptr[matrixIndex] =
//                        boneMatrixTable.ptr[matrixIndex] *
//                        iterator->pBone->invert;
            }
        }
        
        ++iterator;
    }
    calcMotionIK();
}

static mat4 calcGlobalMatrix(MmdMotionBinder::BoneBind *pBindTable, MmdMotionBinder::BoneBind *pTargetBone) {
    if (pTargetBone->pBone->parent) {
        return calcGlobalMatrix(pBindTable, pBindTable + pTargetBone->pBone->parent->getIndex()) * (*pTargetBone->pLocalMatrix);
    } else {
        return (*pTargetBone->pLocalMatrix);
    }
}

void MmdMotionBinder::calcMotionIK() {
    glm::vec4 effectorPos;
    glm::vec4 targetPos;
    
    glm::vec4 localEffectorPos(0, 0, 0, 0);
    glm::vec4 localTargetPos(0, 0, 0, 0);
    
    glm::vec3 localEffectorDir;
    glm::vec3 localTargetDir;
    
    glm::mat4 tmpMatrix;
    
    glm::mat4 invCoord;
    
    glm::vec3 axis;
    glm::mat4 rotation;

    mat4 *pMatrixTable = boneMatrixTable.ptr;
    
    unsafe_array<BoneBind> iterator = binds.iterator();
    while (iterator) {
        PmxPreCalcBone *IKBone = iterator->pBone;
        if (IKBone->self->hasFlag(PmxBone::Flag::IkEnable)) {
            assert(IKBone->self->getIkTargetBoneIndex() >= 0);

            glm::mat4 IKBoneMatrix = calcGlobalMatrix(binds.ptr, iterator.ptr);
            BoneBind *targetBone = binds.ptr + IKBone->self->getIkTargetBoneIndex();
            unsafe_array<PmdIkLink> ikLinkList = IKBone->self->getIkLinkList();

            for (int iterations = 0; iterations < IKBone->self->getIkLoopCount(); iterations++) {
                for (int attentionIndex = 0; attentionIndex < ikLinkList.length; attentionIndex++) {
                    PmdIkLink *IKLink = ikLinkList.ptr + attentionIndex;
                    assert(IKLink->linkBoneIndex >= 0);
                    BoneBind *linkBone = binds.ptr + IKLink->linkBoneIndex;

                    tmpMatrix = calcGlobalMatrix(binds.ptr, targetBone);

                    effectorPos.x = tmpMatrix[3][0];
                    effectorPos.y = tmpMatrix[3][1];
                    effectorPos.z = tmpMatrix[3][2];
                    effectorPos.w = tmpMatrix[3][3];

                    targetPos.x = IKBoneMatrix[3][0];
                    targetPos.y = IKBoneMatrix[3][1];
                    targetPos.z = IKBoneMatrix[3][2];
                    targetPos.w = IKBoneMatrix[3][3];

                    invCoord = glm::inverse(calcGlobalMatrix(binds.ptr, linkBone));

                    localEffectorPos = invCoord * effectorPos;
                    localTargetPos = invCoord * targetPos;

                    localEffectorDir = glm::normalize(glm::vec3(glm::normalize(localEffectorPos)));
                    localTargetDir = glm::normalize(glm::vec3(glm::normalize(localTargetPos)));

                    float p = glm::dot(localEffectorDir, localTargetDir);
                    if (p > 1 - 1.0e-6f) {
                        continue;
                    }

                    // 角度強制
                    float angle = acos(p);
                    angle = std::min(angle, IKBone->self->getIkLimitedRadian());

                    axis = -glm::cross(localTargetDir, localEffectorDir);
                    rotation = glm::rotate(angle, axis);

                    if (IKLink->rotateLimited) {
                        const glm::quat desired_rotation(*linkBone->pLocalMatrix * rotation);
                        const glm::vec3 desired_euler = glm::eulerAngles(desired_rotation);
                        const glm::vec3 clamped_euler = glm::clamp(desired_euler, IKLink->minRadian, IKLink->maxRadian);
                        const glm::quat clamped_rotation(clamped_euler);
//                        eslog("    linkBone[%s] angle(%.2f) euler(%.2f, %.2f, %.2f) min(%.1f, %.1f, %.1f) max(%.1f, %.1f, %.1f)",
//                              linkBone->pBone->self->getName().c_str(),
//                              angle,
//                              clamped_euler.x, clamped_euler.y, clamped_euler.z,
//                              IKLink->minRadian.x, IKLink->minRadian.y, IKLink->minRadian.z,
//                              IKLink->maxRadian.x, IKLink->maxRadian.y, IKLink->maxRadian.z
//                        );

                        linkBone->motionTranslate = vec3((*linkBone->pLocalMatrix)[3][0], (*linkBone->pLocalMatrix)[3][1], (*linkBone->pLocalMatrix)[3][2]);
                        linkBone->motionRotate = clamped_rotation;
                        (*linkBone->pLocalMatrix) = glm::translate(linkBone->motionTranslate) * glm::toMat4(clamped_rotation);
                    }
                    else {
                        linkBone->motionRotate *= quat(angle, axis);
                        (*linkBone->pLocalMatrix) *= rotation;
                    }
                }

                const float errToleranceSq = 0.00001f;
                if (glm::length2(localEffectorPos - localTargetPos) < errToleranceSq) {
                    break;
                }
            }
        }

        ++iterator;
    }


    // IK適用した行列を再度生成する
    iterator = binds.iterator();
    while (iterator) {
//        mat4 rotateLink;
//        if (iterator->pBone->self->hasFlag(PmxBone::Flag::GiveRotation)) {
//            int16_t providedBoneIndex = iterator->pBone->self->getProvidedParentBoneIndex();
//            rotateLink = glm::toMat4(binds.ptr[providedBoneIndex].motionRotate);
//        }

        if (iterator->pBone->parent) {
            // 親行列と適用する
            (*iterator->pGlobalMatrix) = (*binds.ptr[iterator->pBone->parent->getIndex()].pGlobalMatrix) * (*iterator->pLocalMatrix);
//            (*iterator->pGlobalMatrix) = calcGlobalMatrix(binds.ptr, iterator.ptr);
        }

        (*iterator->pVertexMatrix) = (*iterator->pGlobalMatrix) * iterator->pBone->invert;

        ++iterator;
    }
}
    
}
