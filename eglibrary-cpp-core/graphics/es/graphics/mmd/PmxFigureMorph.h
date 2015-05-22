//
// Created by @eaglesakura on 15/05/22.
//

#ifndef ASSEMBLE_PMXFIGUREMORPH_H
#define ASSEMBLE_PMXFIGUREMORPH_H

#include "es/eglibrary.hpp"
#include "PmxMesh.h"

namespace es {
/**
 * メッシュ（頂点）のモーフィング情報
 *
 * 位置 / UVで利用する
 */
struct PmxMeshMorphVertex {
    /**
     * 頂点インデックス
     */
    uint vertexIndex;

    /**
     * 加算量
     */
    vec4 offset;

    /**
     * 頂点の頭から書き込み位置までのオフセット量
     */
    uint offsetBytes;
};

/**
 * ボーンのモーフィング情報
 */
struct PmxBoneMorph {
    /**
     * ボーンインデックス
     */
    uint boneIndex;

    /**
     * 位置遷移
     */
    vec4 translate;

    /**
     * 遷移後の回転
     */
    quat rotate;
};

struct PmxMaterialMorph {
    /**
     * 変形するマテリアル番号
     */
    uint materialIndex;

    vec4 diffuse;

    vec4 specular;

    float shininess;

    vec3 ambient;

    vec4 edgeColor;

    float edgeSize;

    vec4 textureCoefficient;

    vec4 sphereTextureCoefficient;

    vec4 toonTextureCoefficient;
};

/**
 * モーフのグルーピング情報
 */
struct PmxMorphGroup {
    /**
     * グルーピング対象のインデックス
     */
    uint morphIndex;

    /**
     * モーフの比重
     */
    float morphRate;
};

}


#endif //ASSEMBLE_PMXFIGUREMORPH_H
