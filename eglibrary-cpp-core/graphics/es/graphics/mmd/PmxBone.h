//
// Created by @eaglesakura on 15/05/13.
//

#ifndef ASSEMBLE_PMXBONE_H
#define ASSEMBLE_PMXBONE_H

#include <es/math/Vector3.hpp>
#include <es/memory/SafeArray.hpp>
#include <bitset>
#include <vector>
#include <es/math/Matrix.hpp>
#include    "es/eglibrary.hpp"
#include    "es/graphics/math/GlmHelper.hpp"

namespace es {

struct PmdIkLink {
    /**
     * リンクボーンのボーンIndex
     */
    int16_t linkBoneIndex = -1;

    /**
     * 角度制限
     */
    bool rotateLimited = false;

    /**
     * 下限 (x,y,z) -> ラジアン角
     */
    vec3 minRadian;

    /**
     * 上限 (x,y,z) -> ラジアン角
     */
    vec3 maxRadian;
};

/**
 * PMXファイルのボーン情報を管理する
 *
 * 全ボーン情報が含まれているため、それなりに重い
 */
class PmxBone : public Object {
    std::string name;
    std::string nameEng;

    /**
     * ボーン位置
     */
    vec3 position;

    /**
     * 変形階層レベル
     */
    int deformationLevel;

    /**
     * このボーンの番号
     */
    int16_t index = 0;


    /**
     *
     */
    ::std::bitset <8> flags[2];

    /**
     * 親ボーン
     */
    int16_t parentBoneIndex = -1;

    /**
     * 接続先ボーン
     */
    int16_t connectedBoneIndex = -1;

    /**
     * 付与親ボーン
     */
    int16_t providedParentBoneIndex = -1;

    /**
     * 付与率
     */
    float providedRatio = 0;

    /**
     * ボーンの位置オフセット
     */
    vec3 positionOffset;

    /**
     * ボーンの向きベクトル
     */
    vec3 axisDirectionVector;

    vec3 dimentionXDirectionVector;

    vec3 dimentionZDirectionVector;

    int32_t keyValue;

    int16_t ikTargetBoneIndex;

    int16_t ikLoopCount;

    float ikLimitedRadian;

    /**
     * IKリスト
     */
    safe_array<PmdIkLink> ikLinkList;
public:

    PmxBone() { }

    virtual ~PmxBone() { }


    const std::string &getName() const {
        return name;
    }

    void setName(const std::string &name) {
        PmxBone::name = name;
    }

    const std::string &getNameEng() const {
        return nameEng;
    }

    void setNameEng(const std::string &nameEng) {
        PmxBone::nameEng = nameEng;
    }

    const vec3 &getPosition() const {
        return position;
    }

    void setPosition(const vec3 &position) {
        PmxBone::position = position;
    }

    int getDeformationLevel() const {
        return deformationLevel;
    }

    void setDeformationLevel(int deformationLevel) {
        PmxBone::deformationLevel = deformationLevel;
    }

    int16_t getIndex() const {
        return index;
    }

    void setIndex(int index) {
        PmxBone::index = (uint16_t) index;
    }

    void setFlags(uint index, uint8_t bits) {
        flags[index] = ::std::bitset<8>(bits);
    }

    enum Flag {
        ConnectionDisplayMethod = 0,
        RotationEnable,
        TranslateEnable,
        Visible,
        ControlEnable,
        IkEnable,

        GiveRotation = 10,
        GiveTranslation,
        AxisFixed,
        LocalAxis,
        TransformAfterPhysics,
        ExternalParentTransform,
    };

    bool hasFlag(Flag flag) const {
        const int arrayIndex = flag / 10;
        const int bitIndex = flag % 10;

        return flags[arrayIndex][bitIndex];
    }


    int16_t getParentBoneIndex() const {
        return parentBoneIndex;
    }

    void setParentBoneIndex(int parentBoneIndex) {
        PmxBone::parentBoneIndex = parentBoneIndex;
    }

    int16_t getConnectedBoneIndex() const {
        return connectedBoneIndex;
    }

    void setConnectedBoneIndex(int connectedBoneIndex) {
        PmxBone::connectedBoneIndex = connectedBoneIndex;
    }

    int16_t getProvidedParentBoneIndex() const {
        return providedParentBoneIndex;
    }

    void setProvidedParentBoneIndex(int providedParentBoneIndex) {
        PmxBone::providedParentBoneIndex = providedParentBoneIndex;
    }

    float getProvidedRatio() const {
        return providedRatio;
    }

    void setProvidedRatio(float providedRatio) {
        PmxBone::providedRatio = providedRatio;
    }

    const vec3 &getPositionOffset() const {
        return positionOffset;
    }

    void setPositionOffset(const vec3 &positionOffset) {
        PmxBone::positionOffset = positionOffset;
    }

    const vec3 &getAxisDirectionVector() const {
        return axisDirectionVector;
    }

    void setAxisDirectionVector(const vec3 &axisDirectionVector) {
        PmxBone::axisDirectionVector = axisDirectionVector;
    }

    const vec3 &getDimentionXDirectionVector() const {
        return dimentionXDirectionVector;
    }

    void setDimentionXDirectionVector(const vec3 &dimentionXDirectionVector) {
        PmxBone::dimentionXDirectionVector = dimentionXDirectionVector;
    }

    const vec3 &getDimentionZDirectionVector() const {
        return dimentionZDirectionVector;
    }

    void setDimentionZDirectionVector(const vec3 &dimentionZDirectionVector) {
        PmxBone::dimentionZDirectionVector = dimentionZDirectionVector;
    }

    int32_t getKeyValue() const {
        return keyValue;
    }

    void setKeyValue(int keyValue) {
        PmxBone::keyValue = keyValue;
    }

    int16_t getIkTargetBoneIndex() const {
        return ikTargetBoneIndex;
    }

    void setIkTargetBoneIndex(int ikTargetBoneIndex) {
        PmxBone::ikTargetBoneIndex = (int16_t) ikTargetBoneIndex;
    }

    int16_t getIkLoopCount() const {
        return ikLoopCount;
    }

    void setIkLoopCount(int ikLoopCount) {
        PmxBone::ikLoopCount = (int16_t) ikLoopCount;
    }

    float getIkLimitedRadian() const {
        return ikLimitedRadian;
    }

    void setIkLimitedRadian(float ikLimitedRadian) {
        PmxBone::ikLimitedRadian = ikLimitedRadian;
    }

    const safe_array<PmdIkLink> &getIkLinkList() const {
        return ikLinkList;
    }

    void allocIkLinkList(uint ikNum);

    PmdIkLink *getIkLink(uint index) const {
        assert(index < ikLinkList.length);
        return ikLinkList.ptr + index;
    }

    bool hasPreantBone() const {
        return parentBoneIndex >= 0;
    }

    bool hasConnectedBone() const {
        return connectedBoneIndex >= 0;
    }

    bool hasProvidedParentBone() const {
        return providedParentBoneIndex >= 0;
    }
};

typedef std_shared_ptr<PmxBone> MPmxBone;

/**
 * ボーンの事前計算情報
 *
 * ボーンは速度を優先するためにポインタの直接参照を行っているが、
 * 生存はPmxBoneControllerに合わせて保証される。
 */
struct PmxPreCalcBone {
    /**
     * 親ボーン
     */
    PmxBone *parent = nullptr;

    /**
     * 自分位置
     */
    PmxBone *self = nullptr;

    /**
     * 開始位置
     */
    vec3 pos;

    /**
     * ボーンの向き
     */
    vec3 axis;

    /**
     * ボーン終了位置
     */
    vec3 end;

    /**
     * 原点への変換逆行列
     */
    mat4 invert;

    PmxPreCalcBone() { }

    ~PmxPreCalcBone() { }
};

/**
 * PMXボーンの制御を行う
 */
class PmxBoneController : public Object {
    /**
     * 処理対象のボーン一覧
     */
    safe_array<MPmxBone> bones;

    /**
     * 事前計算ボーン一覧
     */
    safe_array<PmxPreCalcBone> preCalcBones;
public:
    PmxBoneController();

    virtual ~PmxBoneController() { }

    /**
     * 必要な情報の事前計算を行う
     */
    virtual void initialize(const std::vector<MPmxBone> &bones);

    /**
     * ボーン数を取得する
     */
    virtual uint getBoneCount() const {
        return bones.length;
    }

    /**
     * 計算済みボーン情報を取得する
     */
    virtual PmxPreCalcBone *getBone(uint index) const {
        return preCalcBones.ptr + index;
    }
};

typedef std_shared_ptr<PmxBoneController> MPmxBoneController;

}

#endif //ASSEMBLE_PMXBONE_H
