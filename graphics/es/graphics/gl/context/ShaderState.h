#pragma once

#include    "es/Graphics.hpp"
#include    "es/graphics/GPU.h"

#include    "es/graphics/gl/context/GLContextUtil.hpp"

namespace es {

class ShaderState : public Object {
    enum {
        /**
         * テクスチャユニットの最大数
         */
                MAX_TEXTURE_UNIT = 32,
    };

    /**
     * glBindTexture()されているテクスチャ情報
     */
    struct {
        /**
         * GLESの定数として、index0〜31までは確保されている。
         * ただし、それがハードウェアとして対応しているかは問わない
         * GL_TEXTURE0〜GL_TEXTURE31に対応
         */
        GLuint textures[MAX_TEXTURE_UNIT];

        /**
         * GL_TEXTURE0〜GL_TEXTURE31に対応
         */
        GLenum targets[MAX_TEXTURE_UNIT];

        /**
         * アクティブ化されているテクスチャユニットを保持する
         */
        uint active;
    } textureContext;

    /**
     * テクスチャユニットの定数をテクスチャ番号に変換する
     */
    inline static uint toTextureIndex(const GLenum textureUnit) {
        return textureUnit - GL_TEXTURE0;
    }

    /**
     * テクスチャ番号をテクスチャユニット定数に変換する
     */
    inline static GLenum toTextureUnit(const uint index) {
        return index + GL_TEXTURE0;
    }

public:
    ShaderState();

    virtual ~ShaderState();

    /**
     * Contextと同期を行う
     */
    void sync();

    /**
     * 指定したテクスチャがバインド済みのユニットを取得する
     */
    inline int getBindedTextureUnit(const GLenum target, const GLuint texture) const {
        for (int i = 0; i < GPU::getMaxTextureUnits(); ++i) {
            if (textureContext.textures[i] == texture && textureContext.targets[i] == target) {
                return i;
            }
        }
        return -1;
    }

    /**
     * テクスチャユニットをActiveにする
     */
    inline bool activeTexture(const uint index) {
        assert(index < GPU::getMaxTextureUnits());
        const uint unit = toTextureUnit(index);
        // 違うユニットがアクティブ化されていたら、アクティブにし直す
        if (unit != textureContext.active) {
            textureContext.active = unit;
            glActiveTexture(unit);
            assert_gl();
            return true;
        }
        return false;
    }

    /**
     * 現在アクティブになっているテクスチャの番号を取得する。
     */
    inline uint getActiveTextureIndex() {
        return toTextureIndex(textureContext.active);
    }

    /**
     * 現在activeになっているテクスチャユニットに対してバインドを行う。
     */
    bool bindTexture(const GLenum target, const GLuint texture);

    /**
     * まだバインドされているかをチェックする
     */
    bool isBindedTexture(const uint index, const GLenum target, const GLuint texture) {
        assert(index < GPU::getMaxTextureUnits());
        return textureContext.textures[index] == texture && textureContext.targets[index] == target;
    }

    /**
     * 指定したテクスチャがバインド済みになっているかを調べる
     */
    bool isBindedTexture(const GLenum target, const GLuint texture) const {
        return getBindedTextureUnit(target, texture) >= 0;
    }

    /**
     * バインドされているテクスチャを一括で解放する
     */
    void unbindTextures(const uint num, const GLuint *textures);

    /**
     * 全てのテクスチャを一括で外す
     */
    void unbindTextures();

    /**
     * 引数のテクスチャがどれかにバインドされている場合、バインドを解除する。
     */
    inline void unbindTexture(const GLuint texture) {
        unbindTextures(1, &texture);
    }

    /**
     * 空いているテクスチャユニット番号を取得する
     * この機能は複数枚テクスチャを使用する場合、持ち回りでテクスチャユニットがオーバーライドされる恐れがある。
     * マルチテクスチャの描画ではこのメソッドを使用しないことを推奨する。
     *
     * @param overrride trueの場合、適当なテクスチャユニットをピックアップして返す。
     */
    int getFreeTextureUnitIndex(const bool overrride);
};

/**
 * Managed
 */
typedef ::std::shared_ptr<ShaderState> MShaderState;

}
