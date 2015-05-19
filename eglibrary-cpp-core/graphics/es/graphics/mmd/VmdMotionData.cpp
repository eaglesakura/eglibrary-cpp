#include <cmath>
#include "VmdMotionData.h"

namespace es {

MVmdBoneMotionData VmdMotionData::findBoneController(std::string name, bool create) {
    for (MVmdBoneMotionData ctrl : boneMotions) {
        if (ctrl->getName() == name) {
            return ctrl;
        }
    }
    
    if (create) {
        // キャッシュが見つからないので新たに生成する
        MVmdBoneMotionData ctrl(new VmdBoneMotionData());
        ctrl->setName(name);
        
        boneMotions.push_back(ctrl);
        
        return ctrl;
    } else {
        return MVmdBoneMotionData();
    }
}

bool VmdBoneMotionData::getKey(const float frame, VmdKeyFrame **start, VmdKeyFrame **end) {
    // check frame
    const uint16_t checkFrame = ceil(frame);

    VmdKeyFrame *before = nullptr;
    VmdKeyFrame *beforeBefore = nullptr;
    for (VmdKeyFrame &key : keys) {
        if (checkFrame <= (float) key.frame) {
            *end = &key;
            if (before) {
                *start = before;
            } else {
                *start = *end;
            }
            return true;
        } else {
            beforeBefore = before;
            before = &key;
        }
    }

    if (before && beforeBefore) {
        *start = beforeBefore;
        *end = before;
        return true;
    } else if (before) {
        *start = *end = before;
        return true;
    }

    return false;
}

bool VmdBoneMotionData::getKey(const float frame, vec3 *position, quat *rotate) {
    VmdKeyFrame *start = nullptr;
    VmdKeyFrame *end = nullptr;

    getKey(frame, &start, &end);
    assert(start);
    assert(end);

    assert(start->frame <= end->frame);

    if (end->frame != start->frame) {
        float weight = (frame - (float) start->frame) / (float) (end->frame - start->frame);
        assert(weight >= 0.0f);
        assert(weight <= 1.0f);

        *position = (end->pos * weight) + (start->pos * (1.0f - weight));
        *rotate = glm::mix(start->rotate, end->rotate, weight);
    } else {
        *position = start->pos;
        *rotate = start->rotate;
    }

    return true;
}

VmdKeyFrame *VmdBoneMotionData::getKey(const int frame) {
//    return &keys[0];
    VmdKeyFrame *before = nullptr;
    for (VmdKeyFrame &key : keys) {
        if (key.frame == (int16_t) frame) {
            return &key;
        } else if (key.frame > frame) {
//            return nullptr;
            return before;
        } else {
            before = &key;
        }
    }
    return nullptr;
}

VmdKeyFrame *VmdBoneMotionData::newKeyFrame(const int frame) {
    VmdKeyFrame key;
    key.frame = frame;
    keys.push_back(key);
    this->maxFrame = std::max(maxFrame, (int16_t) frame);
    return &keys[keys.size() - 1];
}

mat4 VmdMotionData::calcBoneLocalMatrix(int keyFrame, std::string boneName) {
    MVmdBoneMotionData controller = findBoneController(boneName, false);
    if (controller) {
        VmdKeyFrame *key = controller->getKey(keyFrame);

        if (key) {
            mat4 translate = glm::translate(key->pos);
            mat4 rotate = glm::toMat4(key->rotate);

            return translate * rotate;
        } else {
            return mat4();
        }
    } else {
        // モーションが指定されていなければ単位行列
        return mat4();
    }
    
}

VmdKeyFrame *VmdBoneMotionData::getFirstKey() {
    if (keys.empty()) {
        return nullptr;
    } else {
        return &keys[0];
    }
}

void VmdMotionData::insertLoopDummy(int dummyFrame) {
    for (MVmdBoneMotionData boneMotion : boneMotions) {
        boneMotion->insertLoopDummy(dummyFrame, allFrames + 1);
    }
    allFrames++;
}

void VmdBoneMotionData::insertLoopDummy(int refFrame, const uint frame) {
    VmdKeyFrame *dummyFrame = this->newKeyFrame(frame);
    
    VmdKeyFrame *refKey = getKey(refFrame);
    assert(refKey);
    this->getKey((float) refFrame, &dummyFrame->pos, &dummyFrame->rotate);
    memcpy(dummyFrame->bezier, refKey->bezier, sizeof(dummyFrame->bezier));
}

}

