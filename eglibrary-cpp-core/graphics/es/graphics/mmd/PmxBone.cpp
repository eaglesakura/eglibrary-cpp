//
// Created by @eaglesakura on 15/05/13.
//

#include "PmxBone.h"

namespace   es {

void PmxBone::allocIkLinkList(uint ikNum) {
    this->ikLinkList.reserve(ikNum);
}

PmxBoneController::PmxBoneController() {

}


void PmxBoneController::initialize(const std::vector<MPmxBone> &newBones) {
    // メモリを確保する
    this->bones.reserve(newBones.size());
    this->preCalcBones.reserve(newBones.size());

    {
        uint index = 0;
        for (MPmxBone bone : newBones) {
            this->bones[index] = newBones[index];

            PmxPreCalcBone *pBone = preCalcBones.ptr + index;
            pBone->self = this->bones[index].get();

            if (pBone->self->hasPreantBone()) {
                pBone->parent = newBones[pBone->self->getParentBoneIndex()].get();
            }

            pBone->pos = pBone->self->getPosition();
            if (pBone->self->hasConnectedBone()) {
                // 接続先ボーンがあるから、接続先ボーンがendとなる
                pBone->end = newBones[pBone->self->getConnectedBoneIndex()]->getPosition();
            } else {
                // 接続先ボーンが無いから、オフセットがendとなる
                pBone->end = pBone->self->getPosition() + pBone->self->getPositionOffset();
            }
            pBone->axis = es::axis(pBone->pos, pBone->end);
//            pBone->axis = glm::normalize(pBone->end - pBone->end);

            ++index;
        }
    }
}

}