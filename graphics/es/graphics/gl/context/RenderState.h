#pragma once

#include    "es/Graphics.hpp"
#include    "es/math/Rect.hpp"
#include    "es/graphics/Color.hpp"
#include    <vector>

namespace es {

/**
 * キャッシュコントロールするステート情報
 */
enum GLState_e {
    /**
     * カリング ON / Front
     */
            GLState_Cull_Front = 0x1 << 0,

    /**
     * カリング ON / Back
     */
            GLState_Cull_Back = 0x1 << 1,

    /**
     * 深度テストを有効化
     */
            GLState_DepthTest_Enable = 0x1 << 2,

    /**
     * ステンシルテスト有効化
     */
            GLState_StencilTest_Enable = 0x1 << 3,

#if 0 /* TODO! */
    /**
     * Redへ書き込まない
     */
    GLStates_Mask_Red_Disable = 0x1 << 4,

    /**
     * Greenへ書き込まない
     */
    GLStates_Mask_Green_Disable = 0x1 << 5,

    /**
     * Blueへ書き込まない
     */
    GLStates_Mask_Blue_Disable = 0x1 << 6,

    /**
     * Alphaへ書き込まない
     */
    GLStates_Mask_Alpha_Disable = 0x1 << 7,

    /**
     * 深度へ書き込まない
     */
    GLStates_Mask_Depth_Disable = 0x1 << 8,

    /**
     * ステンシルへ書き込まない
     */
    GLStates_Mask_Stencil_Disable = 0x1 << 9,
#endif
    /**
     * 表ポリゴンをデフォルトとは反転する
     */
            GLState_FrontFace_CW = 0x1 << 10,
};

/**
 * states
 */
typedef uint32_t glstates_flags;

enum GLBlendType_e {
    /**
     * 一般的なαブレンドを行う
     */
            GLBlendType_Alpha = 0,

    /**
     * 加算ブレンディングを行う
     */
            GLBlendType_Add,

    /**
     * ブレンドを行わない
     */
            GLBlendType_None,

    /**
     * 不明なブレンド
     * 初回set時に必ず反応するようにしておく
     */
            GLBlendType_Unknown,
};

/**
 * カプセル化したステート設定
 *
 * 基本的には 0 clear状態で使用し、任意で書き換える
 */
struct glstates {
    /**
     * state flags
     */
    glstates_flags flags;

    /**
     * シザーエリア
     */
    RectI16 scissor;

    /**
     * ビューポート
     */
    RectI16 viewport;

    /**
     * glClear color
     */
    Color clear;

    /**
     * レンダリング対象のフレームバッファ
     */
    GLuint framebuffer;

    /**
     * 描画するラインの太さ
     */
    uint8_t lineWidth;

    /**
     * GLBlendType_e
     */
    GLBlendType_e blendType :8;

    /**
     * シザーテストを行わない
     */
    void disableScissor() {
        scissor.left = scissor.right = scissor.top = scissor.bottom = 0;
    }

    /**
     * シザーボックスが0であればシザーを使用しない
     */
    bool isDisableScissor() const {
        return scissor.getAreaSize() == 0;
    }
};

class RenderState : public Object {
protected:
    std::vector<glstates> states;

    /**
     * 現在のステートを取得する
     */
    glstates *get() {
        return &states[states.size() - 1];
    }

    /**
     * 現在のステートを取得する
     */
    const glstates *get() const {
        return &states[states.size() - 1];
    }

public:
    RenderState();

    virtual ~RenderState();

    /**
     * 現在のEGLContext状態に合わせて更新する
     */
    void sync();

    /**
     * Viewport値を更新する
     */
    void viewport(int x, int y, int width, int heidht);

    void clearColorI(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

    void clearColor(float r, float g, float b, float a);

    /**
     *
     */
    float getViewportAspect() const {
        const glstates *cur = get();
        float width = cur->viewport.width();
        float height = cur->viewport.height();
        return width / height;
    }

    /**
     * フレームバッファを使用する
     */
    void bindFramebuffer(GLuint framebuffer);

    /**
     * ライン描画の太さを指定する
     */
    void lineWidth(const float width);

    /**
     * 現在のステートを取得する
     */
    const glstates &getCurrent() const {
        return states[states.size() - 1];
    }

    /**
     * 現在のステートを保存し、新たなステートで上書きする
     */
    void push(const glstates &state);

    /**
     * 現在のステートを保存する。
     */
    void push();

    /**
     * 現在のステートを廃棄し、ステートを以前の状態に戻す
     */
    void pop();

    /**
     * GLのブレンドタイプを指定する
     */
    void setBlendType(const GLBlendType_e type);

    /**
     * レンダリングステートを更新する
     */
    void setFlags(const glstates_flags flags);

    /**
     * ステートを更新する
     */
    void set(const glstates &state);

    /**
     * バッファを全てアンバインドしてクリアーな状態にする
     */
    void unbindBuffers();

    /**
     * このスレッドで握っているリソースを全てアンバインドする
     */
    void unbindResources();

    /**
     * メッシュに関連するバッファをクリアーな状態にする。
     *
     * 主にIBO/VBOの使用・未使用を切り替える際にクリーンし忘れないようにする
     */
    void unbindMeshBuffers();
};

typedef ::std::shared_ptr<RenderState> MRenderState;

}
