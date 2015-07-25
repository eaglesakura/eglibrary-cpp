#include "ImmediateSpriteRenderer.h"
#include <es/asset/IAsset.hpp>

namespace es {

ImmediateSpriteRenderer::ImmediateSpriteRenderer() {

}

bool ImmediateSpriteRenderer::initialize(std::shared_ptr<DeviceContext> context, std::shared_ptr<AssetManager> assets) {
    setDeviceContext(context);

    {
        auto vert = assets->load("shader/SpriteShader.vsh");
        auto frag = assets->load("shader/SpriteShader.fsh");
        this->shader = ShaderProgram::build(util::toString(vert), util::toString(frag), OpenGLSLVersion_100);
        assert(shader);
        if (!shader) {
            return false;
        }

        // uniformを設定する
        {
            uniform.poly_data.setLocation(shader, "poly_data");
            uniform.poly_uv.setLocation(shader, "poly_uv");
            uniform.texture.setLocation(shader, "tex");
            uniform.color.setLocation(shader, "blendColor");
            uniform.aspect.setLocation(shader, "aspect");
            uniform.rotate.setLocation(shader, "rotate");
            uniform.colorOnly.setLocation(shader, "colorOnly");
        }
    }

    if (!quad) {
        quad.reset(new Quad());
        // attrを設定する
        quad->bind();
        quad->updateVertices(nullptr, shader->getAttribLocation("vPosition"), shader->getAttribLocation("vTexCoord"));
    }
    context->getRenderState()->unbindResources();

    return true;
}

void ImmediateSpriteRenderer::setDeviceContext(std::shared_ptr<DeviceContext> context) {
    this->context = context;
}


void ImmediateSpriteRenderer::beginRendering(SpriteRenderer *sender) {
    assert(sender && context && shader && quad);

    auto renderState = context->getRenderState();
    auto state2d = context->createDefaultState2D();
    state2d.viewport = renderState->getCurrent().viewport;
    state2d.scissor = renderState->getCurrent().scissor;
    renderState->push(state2d);
    shader->bind();
    quad->bind();
}

int ImmediateSpriteRenderer::requestRendering(SpriteRenderer *sender, const ISpriteRenderingCallback::RenderingState *state, const uint numInstances, ISpriteRenderingCallback::RenderingInstance *instanceArray) {
    int numRendering = 0;
    if (state->mode == RenderingMode_QuadFill || state->mode == RenderingMode_QuadOutLine) {
        RenderingQuadInstance *quadInstances = (RenderingQuadInstance *) instanceArray;
        for (int i = 0; i < numInstances; ++i) {

            if (quadInstances->texture) {
                // テクスチャにキャストできなければならない
                auto texture = std::dynamic_pointer_cast<Texture>(quadInstances->texture);
                assert(texture);

                uniform.texture.upload(texture, context);
                uniform.poly_uv.upload(quadInstances->srcCoord.left, quadInstances->srcCoord.top,
                                       quadInstances->srcCoord.width(), quadInstances->srcCoord.height());
                uniform.colorOnly.upload(0);
            } else {
                uniform.colorOnly.upload(1);
            }

            {
                const float sizeX = quadInstances->dstQuad.width();
                const float sizeY = quadInstances->dstQuad.height();
                const float sx = quadInstances->dstQuad.left;
                const float sy = quadInstances->dstQuad.top;
                const float translateX = sizeX / 2.0f + sx;
                const float translateY = -sizeY / 2.0f + sy;
                // ポリゴン位置の確定
                uniform.poly_data.upload(translateX, translateY, sizeX, sizeY);
            }
            // ブレンド色を設定する
            uniform.color.upload(quadInstances->color);
            // ポリゴン回転を設定する
            uniform.rotate.upload(quadInstances->rotateRadian);
            // アスペクト比を転送する
            uniform.aspect.upload(state->surfaceAspect);

            quad->rendering();

            ++quadInstances;
        }
    }
    return numRendering;
}

void ImmediateSpriteRenderer::endRendering(SpriteRenderer *sender) {
    assert(sender && context && shader && quad);

    auto renderState = context->getRenderState();

    renderState->unbindBuffers();
    renderState->pop();
}

}