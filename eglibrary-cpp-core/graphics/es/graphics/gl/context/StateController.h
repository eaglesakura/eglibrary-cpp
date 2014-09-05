#ifndef es_grahpics_gl_STATECONTROLLER_H_
#define es_grahpics_gl_STATECONTROLLER_H_

#include    "es/Graphics.hpp"
#include    <vector>

namespace es {

/**
 * キャッシュコントロールするステート情報
 */
enum GLStates_e {

    /**
     * カリングON / CCW
     */
    GLStates_Cull_CCW,

    /**
     * カリングON / CW
     */
    GLStates_Cull_CC,

    /**
     * カリング / Front
     */
    GLStates_Cull_Front,

    /**
     * 深度テストを有効化
     */
    GLStates_DepthTest_Enable,

    /**
     * ステンシルテスト有効化
     */
    GLStates_StencilTest_Enable,

    /**
     * カラーマスク設定
     */
    GLStates_Mask_Red,

    GLStates_Mask_Green,

    GLStates_Mask_Blue,

    GLStates_Mask_Alpha,

    GLStates_Mask_Depth,

    GLStates_Mask_Stencil,
};

enum GLBlendType_e {
    /**
     * 一般的なαブレンドを行う
     */
    GLBlendType_Alpha,

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

class StateController: public Object {
    std::vector<glstates> states;
public:
    StateController();
    virtual ~StateController();

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
    static std_shared_ptr<StateController> current();

    /**
     * 現在のスレッドで使用しなくなった
     */
    static void unuseThisThread();
};

typedef std_shared_ptr<StateController> MStateController;

}

#endif /* STATECONTROLLER_H_ */
