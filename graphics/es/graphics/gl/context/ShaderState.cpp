#include    "ShaderState.h"

namespace es {

ShaderState::ShaderState() {
    // reset texture params
    {
        textureContext.active = false;
        for (int i = 0; i < MAX_TEXTURE_UNIT; ++i) {
            textureContext.textures[i] = 0;
            textureContext.targets[i] = 0;
        }
    }
}

ShaderState::~ShaderState() {

}

void ShaderState::sync() {
    // reset texture params
    {
        textureContext.active = false;
        for (int i = 0; i < MAX_TEXTURE_UNIT; ++i) {
            textureContext.textures[i] = 0;
            textureContext.targets[i] = 0;
        }
    }
}

bool ShaderState::bindTexture(const GLenum target, const GLuint texture) {
    const uint index = getActiveTextureIndex();
    assert(index >= 0 && index < GPU::getMaxTextureUnits());

    const GLuint currentTex = textureContext.textures[index];
    const GLenum currentTarget = textureContext.targets[index];

// 違うテクスチャがユニットに設定されていたら、バインドし直す
    if (currentTex != texture || currentTarget != target) {
        textureContext.textures[index] = texture;
        textureContext.targets[index] = target;
        glBindTexture(target, texture);
        assert_gl();
        return true;
    }

    return false;
}

/**
 * バインドされているテクスチャを一括で解放する
 */
void ShaderState::unbindTextures(const uint num, const GLuint* textures) {
    const uint active = toTextureIndex(textureContext.active);

    for (int n = 0; n < num; ++n) {
        const GLuint texture = textures[n];
        for (int i = 0; i < GPU::getMaxTextureUnits(); ++i) {
            if (textureContext.textures[i] == texture) {
                // テクスチャが一致したからunbind
                activeTexture(i);
                bindTexture(textureContext.targets[i], 0);
            }
        }
    }

    activeTexture(active);
}

/**
 * 全てのテクスチャを一括で外す
 */
void ShaderState::unbindTextures() {
    const uint active = toTextureIndex(textureContext.active);
    for (int i = 0; i < GPU::getMaxTextureUnits(); ++i) {
        activeTexture(i);
        bindTexture(GL_TEXTURE_2D, 0);
    }
    activeTexture(active);
}

int ShaderState::getFreeTextureUnitIndex(const bool overrride) {
    for (int i = 0; i < GPU::getMaxTextureUnits(); ++i) {
        if (textureContext.textures[i] == 0) {
            return i;
        }
    }

    //強制的に持ち回りでテクスチャユニットを上書きする
    if (overrride) {
        static int g_overrideTextureUnitIndex = 0;
        const int unitIndex = ((++g_overrideTextureUnitIndex) % GPU::getMaxTextureUnits());
//            jclogf("texture unit = %d", unitIndex);
        assert(unitIndex < GPU::getMaxTextureUnits());
        return unitIndex;
    } else {
        // 上書きせずにエラーを返す
        return -1;
    }
}

}
