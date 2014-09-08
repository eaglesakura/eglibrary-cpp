#ifndef es_graphics_gl_RENDERSTATE_H_
#define es_graphics_gl_RENDERSTATE_H_

#include    "es/Graphics.hpp"
#include    "es/math/Rect.hpp"
#include    "es/graphics/Color.hpp"
#include    <vector>

namespace es {

/**
 * キャッシュコントロールするステート情報
 */
enum GLStates_e {
    /**
     * カリング ON / Front
     */
    GLStates_Cull_Front,

    /**
     * カリング ON / Back
     */
    GLStates_Cull_Back,

    /**
     * 深度テストを有効化
     */
    GLStates_DepthTest_Enable,

    /**
     * ステンシルテスト有効化
     */
    GLStates_StencilTest_Enable,

    /**
     * Redへ書き込まない
     */
    GLStates_Mask_Red_Disable,

    /**
     * Greenへ書き込まない
     */
    GLStates_Mask_Green_Disable,

    /**
     * Blueへ書き込まない
     */
    GLStates_Mask_Blue_Disable,

    /**
     * Alphaへ書き込まない
     */
    GLStates_Mask_Alpha_Disable,

    /**
     * 深度へ書き込まない
     */
    GLStates_Mask_Depth_Disable,

    /**
     * ステンシルへ書き込まない
     */
    GLStates_Mask_Stencil_Disable,
};

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
    uint64_t flags;

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
    color clear;

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
    bool isDisableScissor() {
        return scissor.getAreaSize() == 0;
    }
};

class RenderState: public Object {
protected:
    std::vector<glstates> states;
    RenderState();
public:
    virtual ~RenderState();

    /**
     * 現在のEGLContext状態に合わせて更新する
     */
    void sync();

    /**
     * Viewport値を更新する
     */
    void viewport(int x, int y, int width, int heidht);

    /**
     * 現在のステートを取得する
     */
    const glstates& get() const {
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
     * ステートを更新する
     */
    void set(const glstates &state);

    /**
     * 2Dレンダリングのデフォルトステートを取得する
     */
    static glstates default2D();

    /**
     * 3Dレンダリングのデフォルトステートを取得する
     */
    static glstates default3D();

    /**
     * 現在のThreadに紐付いたStateを取得する
     */
    static std_shared_ptr<RenderState> current();

    /**
     * 現在のスレッドで使用しなくなった
     */
    static void unuseThisThread();
};

typedef std_shared_ptr<RenderState> MStateController;

}

#endif /* RENDERSTATE_H_ */
