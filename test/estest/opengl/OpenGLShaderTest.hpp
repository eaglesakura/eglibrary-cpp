#pragma once

#include "estest/eglibrary-test.hpp"
#include <es/graphics/gl/resource/ShaderProgram.h>

namespace es {
namespace test {

/**
 * スプライト用のデフォルトシェーダーが正常にビルドできることを確認する
 */
TEST(OpenGLShaderTest, SpriteShaderCompile) {
    ES_OPENGL_UNITTEST();

    auto vsh = AssetManager::load("shader/SpriteShader.vsh");
    auto fsh = AssetManager::load("shader/SpriteShader.fsh");
    ASSERT_TRUE((bool) vsh);
    ASSERT_TRUE((bool) fsh);

    ::std::shared_ptr<ShaderProgram> shader = ShaderProgram::build(util::toString(vsh), util::toString(fsh), OpenGLSLVersion_100);

    ASSERT_TRUE((bool) shader);

    eslog("GLSL Version(%d)", shader->getGLSLVersion());
    ASSERT_TRUE(shader->isES20Program());
}

}
}