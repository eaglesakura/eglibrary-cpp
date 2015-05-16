#include "VmdMotionController.h"

namespace es {

MVmdBoneMotionController VmdMotionController::findBoneController(std::string name, bool create) {
    for (MVmdBoneMotionController ctrl : boneMotions) {
        if (ctrl->getName() == name) {
            return ctrl;
        }
    }
    
    if (create) {
        // キャッシュが見つからないので新たに生成する
        MVmdBoneMotionController ctrl(new VmdBoneMotionController());
        ctrl->setName(name);
        
        boneMotions.push_back(ctrl);
        
        return ctrl;
    } else {
        return MVmdBoneMotionController();
    }
}

VmdKeyFrame *VmdBoneMotionController::getKey(const int frame) {
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

VmdKeyFrame *VmdBoneMotionController::newKeyFrame(const int frame) {
    VmdKeyFrame key;
    key.frame = frame;
    keys.push_back(key);
    
    return &keys[keys.size() - 1];
}

mat4 VmdMotionController::calcBoneLocalMatrix(int keyFrame, std::string boneName) {
    MVmdBoneMotionController controller = findBoneController(boneName, false);
    if (controller) {
        VmdKeyFrame *key = controller->getKey(keyFrame);

        if (key) {
//            eslog("hasKey frame[%d] name(%s) R(%.2f, %.2f, %.2f, %.2f)", keyFrame, boneName.c_str(), key->rotate.x, key->rotate.y, key->rotate.z, key->rotate.w);
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
}

