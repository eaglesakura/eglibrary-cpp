#include    "es/graphics/GPU.h"
#include    "es/graphics/gl/context/RenderState.h"

#include    "es/graphics/gl/context/GLContextUtil.hpp"

using namespace std;

namespace es {

RenderState::RenderState() {
    // 必ず一つは生成されるようにする
    states.push_back(glstates());
    // Stateと同期を行う
    sync();
}

RenderState::~RenderState() {

}

/**
 * 現在のEGLContext状態に合わせて更新する
 */
void RenderState::sync() {
    glstates *cur = get();
    cur->flags = 0;

    GLint temp;
    // depth check
    if (glIsEnabled(GL_DEPTH_TEST)) {
        cur->flags |= GLState_DepthTest_Enable;
    }
    assert_gl();

    // cull
    if (glIsEnabled(GL_CULL_FACE)) {
        glGetIntegerv(GL_CULL_FACE_MODE, &temp);
        if (temp == GL_FRONT) {
            cur->flags |= GLState_Cull_Front;
        } else {
            cur->flags |= GLState_Cull_Back;
        }
    }
    assert_gl();

    // FrontFace
    glGetIntegerv(GL_FRONT_FACE, &temp);
    if (temp == GL_CW) {
        // 表ポリの反転
        cur->flags |= GLState_FrontFace_CW;
    }
    assert_gl();

    // stencil
    if (glIsEnabled(GL_STENCIL_TEST)) {
        cur->flags |= GLState_StencilTest_Enable;
    }
    assert_gl();

    // viewport
    {
        GLint xywh[4] = {0};
        glGetIntegerv(GL_VIEWPORT, xywh);
        assert_gl();
        cur->viewport.setXYWH((int16_t) xywh[0], (int16_t) xywh[1], (int16_t) xywh[2], (int16_t) xywh[3]);
    }
    // scissor
    {
        GLint xywh[4] = {0};
        glGetIntegerv(GL_SCISSOR_BOX, xywh);
        assert_gl();
        cur->scissor.setXYWH((int16_t) xywh[0], (int16_t) xywh[1], (int16_t) xywh[2], (int16_t) xywh[3]);
    }
    // framebuffer
    {
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint *) &cur->framebuffer);
        assert_gl();
    }
    // ブレンドタイプは不明にしておく
    cur->blendType = GLBlendType_Unknown;
}

/**
 * Viewport値を更新する
 */
void RenderState::viewport(int x, int y, int width, int heidht) {
    glstates *cur = get();
    if (cur->viewport.left != (int16_t) x || cur->viewport.top != (int16_t) y || cur->viewport.width() != (int16_t) width || cur->viewport.height() != (int16_t) heidht) {
        cur->viewport.setXYWH(x, y, width, heidht);
        glViewport(x, y, width, heidht);
    }
}

void RenderState::clearColorI(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    clearColor((float) r / 255.0f, (float) g / 255.0f, (float) b / 255.0f, (float) a / 255.0f);
}

void RenderState::clearColor(float r, float g, float b, float a) {
    glstates *cur = get();
    Color rgba = Color::fromRGBAf(r, g, b, a);
    if (rgba != cur->clear) {
        cur->clear = rgba;
        glClearColor(r, g, b, a);
    }
}

/**
 * フレームバッファを使用する
 */
void RenderState::bindFramebuffer(GLuint framebuffer) {
    glstates *cur = get();
    if (cur->framebuffer != framebuffer) {
        cur->framebuffer = framebuffer;
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    }
}

/**
 * ライン描画の太さを指定する
 */
void RenderState::lineWidth(const float width) {
    uint8_t newWidth = (uint8_t) width;
    glstates *cur = get();
    if (cur->lineWidth != newWidth) {
        glLineWidth(width);
        cur->lineWidth = newWidth;
    }
}

/**
 * 現在のステートを保存し、新たなステートで上書きする
 */
void RenderState::push(const glstates &state) {
    // 一つ積み上げる
    push();
    // ステートを更新する
    set(state);
}

/**
 * 現在のステートを保存する。
 */
void RenderState::push() {
    states.push_back(getCurrent());
}

/**
 * 現在のステートを廃棄し、ステートを以前の状態に戻す
 */
void RenderState::pop() {
    // 1つ以上pushされていなければならない。
    assert(states.size() >= 2);

    // 一つ古いステートに直す
    {
        const glstates &old = states[states.size() - 2];
        set(old);
    }
    // ステートを一つ外す
    states.pop_back();
    assert(!states.empty());

}

/**
 * GLのブレンドタイプを指定する
 */
void RenderState::setBlendType(const GLBlendType_e type) {
    glstates *cur = get();

    if (cur->blendType == type) {
        // 差分がないなら何もしない
        return;
    }

    static const GLenum sfactor[] = {GL_SRC_ALPHA, GL_SRC_ALPHA,};
    static const GLenum dfactor[] = {GL_ONE_MINUS_SRC_ALPHA, GL_ONE};

    if (type == GLBlendType_None) {
        // no blend
        glDisable(GL_BLEND);
    } else {
        glEnable(GL_BLEND);
        glBlendFunc(sfactor[type], dfactor[type]);
    }
}

/**
 * レンダリングステートを更新する
 */
void RenderState::setFlags(const glstates_flags flags) {
    const glstates_flags oldFlags = get()->flags;

    // 差分をチェックする
    const glstates_flags diffFlags = oldFlags ^flags;
    if (!diffFlags) {
        // 差分がないなら何もしない
        return;
    }

    if (diffFlags & GLState_DepthTest_Enable) {
        // 深度チェックが切り替わった
        if (flags & GLState_DepthTest_Enable) {
            glEnable(GL_DEPTH_TEST);
        } else {
            glDisable(GL_DEPTH_TEST);
        }
    }

    // back cullingを切り替える
    if (diffFlags & GLState_Cull_Back) {
        if (flags & GLState_Cull_Back) {
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
        } else {
            glDisable(GL_CULL_FACE);
        }
    } else if (diffFlags & GLState_Cull_Front) {
        // frontが切り替わった
        if (flags & GLState_Cull_Front) {
            glEnable(GL_CULL_FACE);
            glCullFace(GL_FRONT);
        } else {
            glDisable(GL_CULL_FACE);
        }
    }

    // 表ポリゴンの向きをチェック
    if (diffFlags & GLState_FrontFace_CW) {
        if (flags & GLState_FrontFace_CW) {
            // 表ポリゴン判定を反転する
            glFrontFace(GL_CW);
        } else {
            // 表ポリゴン判定をデフォルトにする
            glFrontFace(GL_CCW);
        }
    }

    // stencil test
    if (diffFlags & GLState_StencilTest_Enable) {
        // ステンシルチェックが切り替わった
        if (flags & GLState_StencilTest_Enable) {
            glEnable(GL_STENCIL_TEST);
        } else {
            glDisable(GL_STENCIL_TEST);
        }
    }

    // フラグの上書き
    get()->flags = flags;
}

/**
 * ステートを更新する
 */
void RenderState::set(const glstates &state) {
    glstates *cur = get();
    setFlags(state.flags);
    setBlendType(state.blendType);
    bindFramebuffer(state.framebuffer);

    // viewport check
    if (cur->viewport != state.viewport) {
        glViewport(state.viewport.left, state.viewport.top, state.viewport.width(), state.viewport.height());
        cur->viewport = state.viewport;
    }

    // scissor check
    if (cur->scissor != state.scissor) {
        if (state.isDisableScissor()) {
            glDisable(GL_SCISSOR_TEST);
        } else {
            glEnable(GL_SCISSOR_TEST);
            glScissor(state.scissor.left, state.scissor.top, state.scissor.width(), state.scissor.height());
        }
        cur->scissor = state.scissor;
    }

    // clear color
    if (cur->clear != state.clear) {
        glClearColor(state.clear.rf(), state.clear.gf(), state.clear.bf(), state.clear.af());
        cur->clear = state.clear;
    }

    assert_gl();
}

void RenderState::unbindMeshBuffers() {
    if (GPU::isSupport(GPUExtension_VertexArrayObject)) {
        glBindVertexArray(0);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

/**
 * バッファを全てアンバインドしてクリアーな状態にする
 */
void RenderState::unbindBuffers() {
    unbindMeshBuffers();
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    bindFramebuffer(0);
}

void RenderState::unbindResources() {
    unbindBuffers();
    glUseProgram(0);

}

const glstates &RenderState::getCurrent() const {
    return states[states.size() - 1];
}

float RenderState::getViewportAspect() const {
    const glstates *cur = get();
    float width = cur->viewport.width();
    float height = cur->viewport.height();
    return width / height;
}

Vector2f RenderState::getSpritePositionToDevice(const int x2D, const int y2D) const {
    const glstates *cur = get();
    Vector2f result(
            (float) x2D / (float) cur->viewport.width(),
            (float) y2D / (float) cur->viewport.height()
    );
    result.x = (result.x * 2.0f) - 1.0f;
    result.y = -((result.y * 2.0f) - 1.0f);
    return result;
}

float RenderState::getSpriteSurfaceAspect() const {
    return getViewportAspect();
}
}
