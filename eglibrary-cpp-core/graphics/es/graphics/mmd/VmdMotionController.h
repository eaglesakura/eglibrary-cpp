//
// Created by @eaglesakura on 15/05/15.
//

#ifndef ASSEMBLE_VMDMOTIONCONTROLLER_H
#define ASSEMBLE_VMDMOTIONCONTROLLER_H

#include <es/memory/SafeArray.hpp>
#include <vector>
#include "es/eglibrary.hpp"
#include "es/graphics/math/GlmHelper.hpp"

namespace es {

struct VmdKeyFrame {
    /**
     * 打刻されている時刻
     */
    int16_t frame;

    /**
     * 打刻されている内容
     */
    vec3 pos;

    /**
     * 回転角
     */
    quat rotate;

    /**
     * 補間パラメータ
     */
    int8_t bezier[64];
};

/**
 * ボーン単位のアニメーション管理
 */
class VmdBoneMotionController : public Object {
    /**
     * 打刻されているキーフレーム一覧
     */
    std::vector<VmdKeyFrame> keys;

    /**
     * 接続されているボーン名
     */
    std::string name;
public:

    VmdBoneMotionController() { }

    virtual ~VmdBoneMotionController() { }

    /**
     * 新しいキーフレームを打刻する
     */
    VmdKeyFrame *newKeyFrame(const int frame);

    VmdKeyFrame *getKey(const int frame);

    const std::string &getName() const {
        return name;
    }

    void setName(const std::string &name) {
        VmdBoneMotionController::name = name;
    }
};

typedef std_shared_ptr<VmdBoneMotionController> MVmdBoneMotionController;

class VmdMotionController {
    /**
     * ボーン単位のモーション
     */
    std::vector<MVmdBoneMotionController> boneMotions;

    /**
     * 関連されたモデル名
     */
    std::string modelName;
public:
    VmdMotionController() { }


    virtual ~VmdMotionController() { }

    /**
     * ボーンコントロールを取得する
     */
    MVmdBoneMotionController findBoneController(std::string name);

    const std::string &getModelName() const {
        return modelName;
    }

    void setModelName(const std::string &modelName) {
        VmdMotionController::modelName = modelName;
    }
};

typedef std_shared_ptr<VmdMotionController> MVmdMotionController;

}


#endif //ASSEMBLE_VMDMOTIONCONTROLLER_H
