//
// Created by @eaglesakura on 15/05/09.
//

#ifndef ASSEMBLE_PMXMATERIAL_H
#define ASSEMBLE_PMXMATERIAL_H

#include <es/graphics/Color.hpp>
#include    "es/eglibrary.hpp"

namespace es {

class PmxTexture {
    std::string name;
public:

    PmxTexture(const std::string &name) : name(name) { }


    virtual ~PmxTexture() { }
};

typedef std_shared_ptr<PmxTexture> MPmxTexture;

class PmxMaterial : public Object {
public:
    enum SphereMode {
        /**
         * スフィア無効
         */
                None,
        /**
         * 乗算
         */
                Sph,
        /**
         * 加算
         */
                Spa,

        /**
         * サブテクスチャ
         * (追加UV1のx,yをUV参照して通常テクスチャ描画を行う)
         */
                SubTex,
    };

    enum RenderFlag {
    /**
     * 両面描画
     */
        NO_CULLING = 0x1 << 0,

        /**
         * 地面影
         */
        SHADOW_GROUND = 0x1 << 1,

        /**
         * セルフシャドウへの描画を行う
         */
        SELFSHADOW_WRITE = 0x1 << 2,

        /**
         * セルフシャドウマッピングの影響を受ける
         */
        SELFSHADOW_READ = 0x1 << 3,

        /**
         * エッジ描画を行う
         */
        RENDERING_EDGE = 0x1 << 4,

        /**
         * 頂点カラーのレンダリングを行う
         */
        VERTEX_COLOR = 0x1 << 5,

        /**
         * ポイント描画を行う
         */
        RENDERING_POINT = 0x1 << 6,

        /**
         * ライン描画を行う
         */
        RENDERING_LINE = 0x1 << 7,
    };
protected:
    std::string name;
    std::string nameEng;
    Color diffuse;
    /**
     * 反射色
     */
    Color specular;
    /**
     * 反射色係数
     */
    float shininess = 0;
    /**
     * 環境色
     */
    Color ambient;

    Color edgeColor;

    float edgeSize = 0;

    /**
     * ディフューズテクスチャ
     */
    MPmxTexture diffuseTexture;

    /**
     * スフィア指定テクスチャ
     */
    MPmxTexture sphereTexture;

    SphereMode sphereMode;

    /**
     * false : 継続値は個別Toon
     *
     * true:継続値は共有Toon
     */
    bool sharedSphereToon = false;

    /**
     * レンダリングフラグを指定する
     */
    uint16_t renderFlags = 0;

    /**
     * 共有Toonフラグ:false : Toonテクスチャ, テクスチャテーブルの参照Index
     * 共有Toonフラグ:true  : 共有Toonテクスチャ[0～9] -> それぞれ toon01.bmp～toon10.bmp に対応どちらか一方なので注意
     */
    MPmxTexture toonTexture;

    /**
     * ユーザーメモ
     */
    std::string memo;

    /**
     * レンダリング用のインデックス数
     */
    uint indicesCount = 0;


public:

    PmxMaterial() { }

    virtual ~PmxMaterial() { }


    uint getIndicesCount() const {
        return indicesCount;
    }

    void setIndicesCount(uint indicesCount) {
        PmxMaterial::indicesCount = indicesCount;
    }

    const std::string &getMemo() const {
        return memo;
    }

    void setMemo(const std::string &memo) {
        PmxMaterial::memo = memo;
    }

    const MPmxTexture &getToonTexture() const {
        return toonTexture;
    }

    void setToonTexture(const MPmxTexture &toonTexture) {
        PmxMaterial::toonTexture = toonTexture;
    }

    bool isSharedSphereToon() const {
        return sharedSphereToon;
    }

    void setSharedSphereToon(bool sphereToon) {
        PmxMaterial::sharedSphereToon = sphereToon;
    }

    const SphereMode &getSphereMode() const {
        return sphereMode;
    }

    void setSphereMode(const SphereMode &sphereMode) {
        PmxMaterial::sphereMode = sphereMode;
    }

    const MPmxTexture &getSphereTexture() const {
        return sphereTexture;
    }

    void setSphereTexture(const MPmxTexture &sphereTexture) {
        PmxMaterial::sphereTexture = sphereTexture;
    }

    const MPmxTexture &getDiffuseTexture() const {
        return diffuseTexture;
    }

    void setDiffuseTexture(const MPmxTexture &diffuseTexture) {
        PmxMaterial::diffuseTexture = diffuseTexture;
    }

    float getEdgeSize() const {
        return edgeSize;
    }

    void setEdgeSize(float edgeSize) {
        PmxMaterial::edgeSize = edgeSize;
    }

    const Color &getAmbient() const {
        return ambient;
    }

    void setAmbient(const Color &ambient) {
        PmxMaterial::ambient = ambient;
    }

    float getShininess() const {
        return shininess;
    }

    void setShininess(float shininess) {
        PmxMaterial::shininess = shininess;
    }

    const Color &getSpecular() const {
        return specular;
    }

    void setSpecular(const Color &specular) {
        PmxMaterial::specular = specular;
    }

    const Color &getDiffuse() const {
        return diffuse;
    }

    void setDiffuse(const Color &diffuse) {
        PmxMaterial::diffuse = diffuse;
    }

    const std::string &getNameEng() const {
        return nameEng;
    }

    void setNameEng(const std::string &nameEng) {
        PmxMaterial::nameEng = nameEng;
    }

    const std::string &getName() const {
        return name;
    }

    void setName(const std::string &name) {
        PmxMaterial::name = name;
    }


    uint16_t getRenderFlags() const {
        return renderFlags;
    }

    void setRenderFlags(uint16_t renderFlags) {
        PmxMaterial::renderFlags = renderFlags;
    }


    const Color &getEdgeColor() const {
        return edgeColor;
    }

    void setEdgeColor(const Color &edgeColor) {
        PmxMaterial::edgeColor = edgeColor;
    }
};

typedef std_shared_ptr<PmxMaterial> MPmxMaterial;

}


#endif //ASSEMBLE_PMXMATERIAL_H
