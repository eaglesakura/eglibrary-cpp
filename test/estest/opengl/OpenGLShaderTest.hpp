#pragma once

#include <es/graphics/gl/resource/ShaderProgram.h>
#include "estest/eglibrary-test.hpp"

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

    auto shader = ShaderProgram::build(util::toString(vsh).c_str(), util::toString(fsh).c_str());

    ASSERT_TRUE((bool) shader);
}

}
}