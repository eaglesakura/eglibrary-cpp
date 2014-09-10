#ifndef es_graphics_gl_TEXTUREUNIFORM_HPP_
#define es_graphics_gl_TEXTUREUNIFORM_HPP_

#include    "es/graphics/gl/resource/Texture.h"
#include    "es/graphics/gl/shader/UniformBase.hpp"
#include    "es/memory/SafeArray.hpp"

namespace es {

/**
 * テクスチャのUniformをサポートする
 * 無効なUniform値（コンパイラ最適化で削除された等）の場合はアップロードを無視する
 */
class TextureUniform: public UniformBase {
    int bindUnit;

public:
    TextureUniform() {
        bindUnit = -1;
    }

    TextureUniform(MShaderProgram program, const char *name) {
        bindUnit = -1;
        setLocation(program, name);
    }

    ~TextureUniform() {

    }

    /**
     * GPUにアップロードを行う
     */
    bool upload(MTexture tex, MDeviceContext context) {
        assert(tex);

        if (!valid()) {
            return false;
        }

        MShaderState state = context->getShaderState();
        int texUnit = -1;
        // バインドされていなければ、テクスチャをバインドする
        if (!tex->isBinded(&texUnit, state)) {
            texUnit = tex->bind(context);
        }

        if (bindUnit != texUnit) {
            glUniform1i(location, texUnit);
            assert_gl();
            bindUnit = texUnit;
            return true;
        }
        return false;
    }
};

/**
 * テクスチャのUniformをサポートする
 * 無効なUniform値（コンパイラ最適化で削除された等）の場合はアップロードを無視する
 */
class TextureArrayUniform: public UniformBase {
    safe_array<int> bindUnits;
public:
    TextureArrayUniform() {
        bindUnits.alloc(GPUCapacity::getMaxTextureUnits());
        for (int i = 0; i < bindUnits.length; ++i) {
            bindUnits[i] = i;
        }
    }

    TextureArrayUniform(MShaderProgram program, const char *name) {
        bindUnits.alloc(GPUCapacity::getMaxTextureUnits());
        for (int i = 0; i < bindUnits.length; ++i) {
            bindUnits[i] = i;
        }
        setLocation(program, name);
    }

    ~TextureArrayUniform() {
    }

    /**
     * アップロードを行う
     */
    bool upload(MTexture *textures, const uint num, MDeviceContext context) {
        if (!valid()) {
            return false;
        }

        assert(textures);

        for (int i = 0; i < num; ++i) {
            textures[i]->bind(i, context);
        }

        glUniform1iv(getLocation(), num, bindUnits.ptr);
        return true;
    }
};

}

#endif /* TEXTUREUNIFORM_HPP_ */
