//
// Created by @eaglesakura on 15/05/08.
//

#ifndef ASSEMBLE_PMXMESH_H
#define ASSEMBLE_PMXMESH_H

#include    "es/eglibrary.hpp"
#include <es/math/Vector3.hpp>
#include <es/math/Vector2.hpp>
#include <es/memory/SafeArray.hpp>
#include <es/math/Vector4.hpp>

namespace es {

/**
 * PMXフィギュア内でスキニングの対象となる頂点情報を格納する
 *
 * このバッファはソフトウェアスキニングで動的に制御やコピーが頻繁に行われる
 */
struct PmxDynamicVertex {
    /**
     * 変形された位置情報
     */
    Vector3f pos;

    /**
     * 変形された法線情報
     */
    Vector3f normal;
};

/**
 * スキニング計算に必要なメタ情報を格納した頂点情報
 *
 * 主にボーン情報が格納される。
 */
struct PmxMetaVertex {
    enum {
        MAX_BONE = 4,
    };

    enum BoneType {
        BDEF1 = 0,
        BDEF2 = 1,
        BDEF4 = 2,
        SDEF = 3,
    };

    /**
     * ボーンの変形方式
     * 0:BDEF1 1:BDEF2 2:BDEF4 3:SDEF
     */
    int boneType;

    /**
     * 実際はunsignedな値が格納される。
     * 影響を与えない場合、weightを0に指定する。
     *
     * 現実的に考えて、ボーン数は16bit数に収まるはずである。
     */
    int16_t boneIndices[MAX_BONE];

    /**
     * ボーンの影響値
     * 0.0〜1.0が与えられる。
     */
    float_t boneWeights[MAX_BONE];

    struct {
        /**
         * SDEF-C値(x,y,z)
         */
        Vector3f c;

        /**
         * SDEF-R0値(x,y,z)
         */
        Vector3f r0;

        /**
         * SDEF-R1値(x,y,z) ※修正値を要計算
         */
        Vector3f r1;
    } boneSdef;

};

/**
 * 静的に扱われる頂点情報
 */
struct PmxStaticVertex {
    /**
     * Edge倍率
     */
    float edgeMagnification;

    /**
     * 宣言的には1つだが、末尾にPMX固有の追加UV数が付与される場合がある。
     */
    Vector2f uv;

    /**
     * 追加UV
     *
     * 最大4つの追加UV座標を持つことになる。
     */
    Vector4f extraUv[0];
};

/**
 * レンダリング用メッシュ情報
 */
class PmxMesh : public Object {
    /**
     * スキニング処理の影響を受けない頂点
     */
    safe_array<uint8_t> staticVertices;

    /**
     * 保持しているUV座標数
     */
    uint numExtraUV = 1;

    /**
     * メタ情報
     */
    safe_array<PmxMetaVertex> metaVertices;

    /**
     * スキニング処理の影響を受ける頂点
     */
    safe_array<PmxDynamicVertex> dynamicVertices;

    /**
     * AABB最小位置
     */
    Vector3f minPosition;

    /**
     * AABB最大位置
     */
    Vector3f maxPosition;
public:
    PmxMesh();

    virtual ~PmxMesh();

    /**
     * 頂点を確保する
     */
    virtual void allocVertices(const uint numVertices, const uint numExtraUV);

    /**
     * 固定頂点の1頂点のサイズ(byte)を取得する
     */
    virtual uint getStaticVertexBytes() const {
        return sizeof(PmxStaticVertex) + (sizeof(Vector4f) * numExtraUV);
    }

    /**
     * 固定頂点情報のポインタを取得する
     *
     * 保持しているUV座標数によって、1頂点の大きさが変化することに注意する。
     */
    virtual uint8_t *getStaticVerticesPointer() const {
        return staticVertices.ptr;
    }

    /**
     * 固定頂点情報を廃棄する
     *
     * 頂点バッファに転送済みである場合、RAM側の情報は不要になるので、削除しても構わない。
     */
    virtual void clearStaticVertices() {
        staticVertices.clear();
    }

    /**
     * メッシュに含まれる頂点数を取得する
     */
    virtual uint32_t getVertexCount() const {
        return dynamicVertices.length;
    }

    /**
     * 動的制御用バッファのバイト数を取得する
     *
     * レンダリング側はこの大きさの格納先メモリを生成する必要がある。
     */
    virtual uint32_t getDynamicVertexBufferBytes() const {
        return sizeof(PmxDynamicVertex) * dynamicVertices.length;
    }

    virtual PmxDynamicVertex *getDynamicVerticesPointer() const {
        return dynamicVertices.ptr;
    }

    virtual PmxMetaVertex *getMetaVerticesPointer() const {
        return metaVertices.ptr;
    }


    virtual const Vector3f &getMinPosition() const {
        return minPosition;
    }

    virtual void setMinPosition(const Vector3f &minPosition) {
        PmxMesh::minPosition = minPosition;
    }

    virtual const Vector3f &getMaxPosition() const {
        return maxPosition;
    }

    virtual void setMaxPosition(const Vector3f &maxPosition) {
        PmxMesh::maxPosition = maxPosition;
    }
};

/**
 * Managed
 */
typedef std_shared_ptr<PmxMesh> MPmxMesh;

}


#endif //ASSEMBLE_PMXMESH_H
