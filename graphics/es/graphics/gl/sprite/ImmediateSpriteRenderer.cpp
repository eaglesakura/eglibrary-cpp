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
        shader->bind();
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

std::shared_ptr<ISpriteRenderingCallback::RenderingContext> ImmediateSpriteRenderer::newRenderingContext(SpriteRenderer *sender) const {
    return std::shared_ptr<ISpriteRenderingCallback::RenderingContext>(new ISpriteRenderingCallback::RenderingContext());
}

void ImmediateSpriteRenderer::beginRendering(SpriteRenderer *sender) {
    assert(sender && context && shader && quad);

    auto renderState = context->getRenderState();
    renderState->push(context->createDefaultState2D());
    shader->bind();
    quad->bind();
}

bool ImmediateSpriteRenderer::requestRendering(SpriteRenderer *sender, const std::shared_ptr<RenderingContext> &context) {

    return false;
}

void ImmediateSpriteRenderer::endRendering(SpriteRenderer *sender) {
    assert(sender && context && shader && quad);

    auto renderState = context->getRenderState();

    renderState->unbindBuffers();
    renderState->pop();
}

}