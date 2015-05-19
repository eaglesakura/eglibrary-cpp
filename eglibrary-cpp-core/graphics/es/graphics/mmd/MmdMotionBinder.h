#ifndef ASSEMBLE_VMDMOTIONBINDER_H
#define ASSEMBLE_VMDMOTIONBINDER_H

#include "es/eglibrary.hpp"
#include "PmxBone.h"
#include "VmdMotionData.h"

namespace es {

/**
 * 1つのPMXに対してモーションを割り当てる。
 *
 * モーフィング管理等の計算も行う。
 *
 * キャッシュを利用するため、高速に動作する。
 *
 * このオブジェクトを精製後、Boneの追加や削除を行ってはいけない。
 * オブジェクト内部でPmxBoneControllerインスタンスを握ることで、各種メモリの生存を保証する
 */
class MmdMotionBinder {
public:
    /**
     * モーション端の制御設定
     */
    enum MotionFillType {
        Loop,
        Stop,
    };
    struct BoneBind {
        /**
         * リンクされたオリジナルBone
         */
        PmxPreCalcBone *pBone = nullptr;

        /**
         *
         */
        VmdKeyFrame *startFrame = nullptr;

        /**
         *
         */
        VmdKeyFrame *endFrame = nullptr;

        /**
         * リンク対象のボーンモーション
         */
        MVmdBoneMotionData boneMotion;

        /**
         * ローカル座標変換行列
         */
        mat4 *pLocalMatrix = nullptr;

        /**
         * グローバル座標変換行列
         */
        mat4 *pGlobalMatrix = nullptr;

        /**
         * 頂点変換行列 / 逆行列適用済み。
         */
        mat4 *pVertexMatrix = nullptr;

        vec3 motionTranslate;

        quat motionRotate;
    };

private:
    /**
     * ボーンごとのローカル行列テーブル
     */
    safe_array<mat4> localMatrixTable;

    /**
     * ボーンのグローバル構成行列テーブル
     */
    safe_array<mat4> boneMatrixTable;

    /**
     * 実際に頂点に適用する行列テーブル
     */
    safe_array<mat4> vertexMatrixTable;

    /**
     * 事前計算されたバインドデータ
     *
     * PMXのボーン数だけ構築される。
     */
    safe_array<BoneBind> binds;

    /**
     * 管理対象のボーン構成
     */
    MPmxBoneData boneData;

    /**
     * バインド対象のモーション情報
     */
    MVmdMotionData motionData;

    /**
     * 現在の再生対象モーションフレーム
     */
    float currentMotionFrame = 0.0f;

    /**
     * ループする場合の起点設定
     */
    struct {
        float start = 0;
        float end = 0;
    } loopRange;

    /**
     * デフォルトはモーションループ
     */
    MotionFillType motionFillType = Loop;


    /**
     * IKの更新を行う
     */
    void calcMotionIK();

public:

    MmdMotionBinder();

    const MPmxBoneData &getBoneData() const {
        return boneData;
    }

    const MVmdMotionData &getMotionData() const {
        return motionData;
    }

    /**
     * ボーンとモーションのバインドを行う
     */
    virtual void bind(const MPmxBoneData &boneData, const MVmdMotionData &motionData);

    float getMotionFrame() const {
        return currentMotionFrame;
    }

    /**
     * モーションを指定フレームに変更する
     */
    void setMotionFrame(float currentMotionFrame);

    /**
     * モーションを指定フレーム遷移させる
     */
    void offsetMotionFrame(float offset) {
        setMotionFrame(currentMotionFrame + offset);
    }


    MotionFillType getMotionFillType() const {
        return motionFillType;
    }

    void setMotionFillType(const MotionFillType &motionFillType) {
        MmdMotionBinder::motionFillType = motionFillType;
    }

    /**
     * アニメーションが指定したフレーム内でループするようにする
     *
     * 範囲は start <= フレーム < end
     */
    void setLoopRange(float start, float end) {
        loopRange.start = start;
        loopRange.end = end;
    }

    /**
     * モーション情報の計算を行う
     */
    virtual void calcMotion();

    /**
     * ボーンのグローバル移動行列を取得する。
     *
     * これは原点からボーンへの遷移行列であるため、頂点に適用するとあらぬ位置に頂点が移動してしまうので注意する。
     */
    virtual unsafe_array<mat4> getBoneMatrixTable() const {
        return boneMatrixTable.iterator();
    }

    /**
     * 頂点のグローバル移動行列を取得する。
     *
     * これは頂点の逆行列を含むため、頂点に適用することでモデルのモーションとして利用できる。
     */
    virtual unsafe_array<mat4> getVertexMatrixTable() const {
        return vertexMatrixTable.iterator();
    }
};

typedef std_shared_ptr<MmdMotionBinder> MMmdMotionBinder;

}


#endif //ASSEMBLE_VMDMOTIONBINDER_H
