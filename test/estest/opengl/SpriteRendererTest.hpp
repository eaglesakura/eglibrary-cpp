#pragma once

#include "estest/eglibrary-test.hpp"
#include "es/graphics/common/2d/SpriteRenderer.h"
#include "es/graphics/gl/resource/ShaderProgram.h"
#include "es/graphics/gl/sprite/ImmediateSpriteRenderer.h"
#include "es/math/Math.hpp"

namespace es {

namespace test {

/**
 * 正常にデバイス座標に対して射影出来るることを確認する
 */
TEST(SpriteRendererTest, DeviceProjection) {
    ES_OPENGL_UNITTEST();
    
    sp<DeviceContext> context = DeviceContext::current();
    // 結果を計算しやすいViewportに設定
    auto renderState = context->getRenderState();

    {
        renderState->viewport(0, 0, 200, 100);

        Vector2f leftTop = renderState->getSpritePositionToDevice(0, 0);
        Vector2f rightBottom = renderState->getSpritePositionToDevice(200, 100);
        Vector2f center = renderState->getSpritePositionToDevice(100, 50);

        ASSERT_TRUE(es::equals(leftTop.x, -1.0f));
        ASSERT_TRUE(es::equals(leftTop.y, 1.0f));

        ASSERT_TRUE(es::equals(rightBottom.x, 1.0f));
        ASSERT_TRUE(es::equals(rightBottom.y, -1.0f));

        ASSERT_TRUE(es::equals(center.x, 0.0f));
        ASSERT_TRUE(es::equals(center.y, 0.0f));
    }

    class TestingSpriteRenderer : public ImmediateSpriteRenderer {
    public:
        int callbackNum = 0;

        virtual int requestRendering(SpriteRenderer *sender, const RenderingState *state, const uint numInstances, RenderingInstance *instanceArray) {
            int result = ImmediateSpriteRenderer::requestRendering(sender, state, numInstances, instanceArray);

            ++callbackNum;
            assert(es::equals(state->surfaceAspect, 200.0f / 100.0f));

            return result;
        }

    };

    sp<SpriteRenderer> spriteRenderer(new SpriteRenderer());
    sp<TestingSpriteRenderer> callback(new TestingSpriteRenderer());
    callback->initialize(context, IProcessContext::getInstance()->getAssetManager());

    spriteRenderer->setDisplay(context->getRenderState());
    spriteRenderer->setCallback(callback);

    spriteRenderer->begin();
    spriteRenderer->renderingRect(0, 0, 200, 100, 0xFFFFFFFF);
    spriteRenderer->renderingRect(0, 0, 0, 100, 0xFFFFFFFF);
    spriteRenderer->renderingRect(0, 0, 200, 0, 0xFFFFFFFF);
    spriteRenderer->end();

    // 無駄なコールバックが呼ばれていないことを確認する
    ASSERT_EQ(callback->callbackNum, 1);
}
    
}
    
}