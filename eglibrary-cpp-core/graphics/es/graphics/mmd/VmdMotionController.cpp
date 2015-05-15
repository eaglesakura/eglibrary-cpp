#include "VmdMotionController.h"

namespace es {

MVmdBoneMotionController VmdMotionController::findBoneController(std::string name) {
    for (MVmdBoneMotionController ctrl : boneMotions) {
        if (ctrl->getName() == name) {
            return ctrl;
        }
    }

    // キャッシュが見つからないので新たに生成する
    MVmdBoneMotionController ctrl(new VmdBoneMotionController());
    ctrl->setName(name);

    boneMotions.push_back(ctrl);

    return ctrl;
}

VmdKeyFrame *VmdBoneMotionController::getKey(const int frame) {
    for (VmdKeyFrame &key : keys) {
        if (key.frame == frame) {
            return &key;
        } else if (key.frame > frame) {
            return nullptr;
        }
    }
    return nullptr;
}

VmdKeyFrame *VmdBoneMotionController::newKeyFrame(const int frame) {
    VmdKeyFrame key;
    key.frame = frame;
    keys.push_back(key);

    return &keys[keys.size() - 1];
}

}

