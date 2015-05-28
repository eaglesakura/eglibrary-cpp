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
    if (keys.size() == 1) {
        *start = *end = &keys[0];
        return true;
    }

    // check frame
    const uint16_t checkFrame = (uint16_t) ceil(frame);

    VmdKeyFrame *before = nullptr;
    VmdKeyFrame *beforeBefore = nullptr;
    for (VmdKeyFrame &key : keys) {
        if (key.frame >= checkFrame) {
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

    if (!beforeBefore) {
        *start = *end = before;
    } else {
        *start = beforeBefore;
        *end = before;
    }

    return true;
}

bool VmdBoneMotionData::getKey(const float frame, vec3 *position, quat *rotate) {
    VmdKeyFrame *start = nullptr;
    VmdKeyFrame *end = nullptr;

    getKey(frame, &start, &end);

    assert(start);
    assert(end);
    assert(start->frame <= end->frame);

    if (frame <= (float) start->frame) {
        // 開始フレームより前であるなら、開始フレームに合わせる
        *position = start->pos;
        *rotate = start->rotate;
    } else if (frame >= (float) end->frame) {
        // 終了フレームより後であるなら、終了フレームに合わせる
        *position = end->pos;
        *rotate = end->rotate;
    } else /* if (end->frame != start->frame) */ {
        float weight = (frame - (float) start->frame) / (float) (end->frame - start->frame);
        assert(weight > 0.0f);
        assert(weight < 1.0f);
        *position = (end->pos * weight) + (start->pos * (1.0f - weight));
        *rotate = glm::normalize(glm::mix(glm::normalize(start->rotate), glm::normalize(end->rotate), weight));
//        *rotate = glm::slerp(end->rotate, start->rotate, weight);
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

VmdKeyFrame *VmdBoneMotionData::newKeyFrame(const int newFrame) {
#ifdef DEBUG
    if (!keys.empty()) {
        // 重複するフレームが存在しないことを確認する
        for (const auto &key : keys) {
            assert(key.frame != newFrame);
        }
    }
#endif

    VmdKeyFrame key;
    key.frame = newFrame;
    keys.push_back(key);

    // MEMO : データの都合上か、キーはランダムな順番に打ち込まれている可能性があるため、逐次ソートが必要になる。
    // sorting
    std::sort(keys.begin(), keys.end(), [](const VmdKeyFrame &lhs, const VmdKeyFrame &rhs) -> bool {
        return lhs.frame < rhs.frame;
    });
    // 最初のフレーム < 最後のフレームであることを保証する
    assert(keys[0].frame <= keys[keys.size() - 1].frame);
    this->maxFrame = std::max(maxFrame, (int16_t) newFrame);

    for (auto &key : keys) {
        if (key.frame == newFrame) {
            return &key;
        }
    }
    // ここに到達してはいけない
    assert(false);
    return nullptr;
//    return &keys[keys.size() - 1];
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

