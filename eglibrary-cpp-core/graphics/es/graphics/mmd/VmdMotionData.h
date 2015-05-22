#ifndef ASSEMBLE_VMDMOTIONCONTROLLER_H
#define ASSEMBLE_VMDMOTIONCONTROLLER_H

#include <es/memory/SafeArray.hpp>
#include <vector>
#include <map>
#include "es/eglibrary.hpp"
#include "es/graphics/math/GlmHelper.hpp"
#include "es/graphics/mmd/PmxBone.h"

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
 *
 * オリジナル情報を管理する
 */
class VmdBoneMotionData : public Object {
    /**
     * 打刻されているキーフレーム一覧
     */
    std::vector<VmdKeyFrame> keys;

    /**
     * 接続されているボーン名
     */
    std::string name;

    /**
     * モーションの最大フレーム数
     */
    int16_t maxFrame = 0;
public:
    VmdBoneMotionData() { }

    virtual ~VmdBoneMotionData() { }

    /**
     * 新しいキーフレームを打刻する
     *
     * 注意) 返却されたポインタは後のキー追加や整列によって無効になる可能性が高いので、絶対に保存してはいけない。
     */
    VmdKeyFrame *newKeyFrame(const int newFrame);

    /**
     * ループ処理用のダミーフレームを挿入する
     * 指定したフレーム番目に、refFrameをダミーとして仕込む。
     */
    void insertLoopDummy(const int refFrame, const uint frame);

    /**
     * 補完用のキーフレーム情報を取得する
     */
    bool getKey(const float frame, VmdKeyFrame **start, VmdKeyFrame **end);

    /**
     * 補完されたキーフレーム情報を取得する
     */
    bool getKey(const float frame, vec3 *position, quat *rotate);

    VmdKeyFrame *getKey(const int frame);

    /**
     * 最初のキーフレームを検索する
     *
     * キーフレームが無い場合はnullptr
     */
    VmdKeyFrame *getFirstKey();

    const std::string &getName() const {
        return name;
    }

    void setName(const std::string &name) {
        VmdBoneMotionData::name = name;
    }


    int16_t getMaxFrame() const {
        return maxFrame;
    }
};

typedef std_shared_ptr<VmdBoneMotionData> MVmdBoneMotionData;

class VmdMotionData {
    /**
     * ボーン単位のモーション
     */
    std::vector<MVmdBoneMotionData> boneMotions;

    /**
     * アニメーションの最大フレーム数
     */
    int32_t allFrames = 0;

    /**
     * 関連されたモデル名
     */
    std::string modelName;
public:
    VmdMotionData() { }


    virtual ~VmdMotionData() { }

    /**
     * ボーンコントロールを取得する
     */
    MVmdBoneMotionData findBoneController(std::string name, bool create = true);

    const std::string &getModelName() const {
        return modelName;
    }

    void setModelName(const std::string &modelName) {
        VmdMotionData::modelName = modelName;
    }


    int32_t getAllFrames() const {
        return allFrames;
    }

    void setAllFrames(int32_t allFrames) {
        VmdMotionData::allFrames = allFrames;
    }

    /**
     * モーションループを行う際、最初と最後を結ぶためのダミーフレームを挿入する
     */
    void insertLoopDummy(int dummyFrame);

    /**
     * ボーン用のローカル行列を取得する
     */
    mat4 calcBoneLocalMatrix(int keyFrame, std::string boneName);
};

typedef std_shared_ptr<VmdMotionData> MVmdMotionData;

}


#endif //ASSEMBLE_VMDMOTIONCONTROLLER_H
