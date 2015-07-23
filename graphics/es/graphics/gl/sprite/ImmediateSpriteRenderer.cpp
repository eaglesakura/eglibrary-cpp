#include "ImmediateSpriteRenderer.h"

namespace es {

ImmediateSpriteRenderer::ImmediateSpriteRenderer() {

}

void ImmediateSpriteRenderer::initialize(std::shared_ptr<DeviceContext> context, std::shared_ptr<AssetManager> assets) {

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