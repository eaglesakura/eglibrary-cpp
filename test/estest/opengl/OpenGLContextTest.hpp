#pragma once

#include "estest/eglibrary-test.hpp"

namespace es {
namespace test {

/**
 * ContextがThreadに対して正常に初期化される
 */
TEST(OpenGLContextTest, InitContext) {
    ES_OPENGL_UNITTEST();

    ASSERT_TRUE((bool) DeviceContext::current());
    ASSERT_EQ(DeviceContext::getContextNum(), 1);
}

/**
 * GPUの内容が正常に取得できていることを確認する
 */
TEST(OpenGLContextTest, GPUCapacityCheck) {
    ES_OPENGL_UNITTEST();

    ASSERT_NE(GPU::getGPUFamily(), GPUFamily_Unknown);
    ASSERT_NE(GPU::getGLVersion(), OpenGLVersion_Unknown);
}

}
}