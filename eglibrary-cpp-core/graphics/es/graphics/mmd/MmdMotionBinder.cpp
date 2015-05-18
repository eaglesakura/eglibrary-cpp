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

    for (uint i = 0; i < binds.length; ++i) {
        // ボーンの基本データを更新する
        BoneBind *pBind = binds.ptr + i;

        pBind->pBone = boneData->getBone(i);
        assert(pBind->pBone);

        pBind->boneMotion = motionData->findBoneController(pBind->pBone->self->getName(), false);
        eslog("    PmxBone[%d] name(%s) -> %s", i, pBind->pBone->self->getName().c_str(), (bool) pBind->boneMotion ? "Link" : "Skip");
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
 */
void MmdMotionBinder::calcMotion() {
//    eslog("MotionFrame(%.1f)", currentMotionFrame);
    unsafe_array<BoneBind> iterator = binds.iterator();
    while (iterator) {
        const uint matrixIndex = iterator->pBone->self->getIndex();
        if (iterator->boneMotion) {
            vec3 motionPos;
            quat motionRotate;
            iterator->boneMotion->getKey(currentMotionFrame, &motionPos, &motionRotate);
            mat4 motion =
                    glm::translate(motionPos) *
                    glm::toMat4(motionRotate);

            // 親の有無で分ける
            if (iterator->pBone->parent) {
                // 親が存在するなら、親の結果に従う
                const uint parentIndex = iterator->pBone->parent->getIndex();

                boneMatrixTable.ptr[matrixIndex] =
                        boneMatrixTable.ptr[parentIndex] *
                        glm::translate(iterator->pBone->offset) *
                        motion;

                vertexMatrixTable.ptr[matrixIndex] =
                        boneMatrixTable.ptr[matrixIndex] *
                        iterator->pBone->invert;

            } else {
                // 親が存在しないならモーションを直接適用する
                boneMatrixTable.ptr[matrixIndex] = motion;
                vertexMatrixTable.ptr[matrixIndex] = motion * iterator->pBone->invert;
            }
        } else {

            // 親の有無で分ける
            // 親が無いなら単位行列のママ何もしない
            if (iterator->pBone->parent) {
                // 親が存在するなら、親の結果に従う
                const uint parentIndex = iterator->pBone->parent->getIndex();

                boneMatrixTable.ptr[matrixIndex] =
                        boneMatrixTable.ptr[parentIndex] *
                        glm::translate(iterator->pBone->offset);

                vertexMatrixTable.ptr[matrixIndex] =
                        boneMatrixTable.ptr[matrixIndex] *
                        iterator->pBone->invert;

            }
        }

        ++iterator;
    }
}
}
