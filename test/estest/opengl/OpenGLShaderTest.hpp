#pragma once

#include <es/graphics/gl/resource/ShaderProgram.h>
#include "estest/eglibrary-test.hpp"

namespace es {
namespace test {

TEST(OpenGLShaderTest, SpriteShaderCompile) {
    ES_OPENGL_UNITTEST();

    auto vsh = debug::TestAssetManager::load("shader/SpriteShader.vsh");
    auto fsh = debug::TestAssetManager::load("shader/SpriteShader.fsh");
    ASSERT_TRUE((bool) vsh);
    ASSERT_TRUE((bool) fsh);

    auto shader = ShaderProgram::build(util::toString(vsh).c_str(), util::toString(fsh).c_str());

    ASSERT_TRUE((bool) shader);
}

}
}