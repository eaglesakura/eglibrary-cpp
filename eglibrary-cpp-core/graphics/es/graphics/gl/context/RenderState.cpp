#include    "es/graphics/gl/context/RenderState.h"
#include    <map>

using namespace std;

namespace es {

RenderState::RenderState() {

}

RenderState::~RenderState() {

}

/**
 * 現在のEGLContext状態に合わせて更新する
 */
void RenderState::sync() {

}

/**
 * Viewport値を更新する
 */
void RenderState::viewport(int x, int y, int width, int heidht) {

}

/**
 * 現在のステートを保存し、新たなステートで上書きする
 */
void RenderState::push(const glstates &state) {

}

/**
 * 現在のステートを保存する。
 */
void RenderState::push() {

}

/**
 * 現在のステートを廃棄し、ステートを以前の状態に戻す
 */
void RenderState::pop() {

}

/**
 * ステートを更新する
 */
void RenderState::set(const glstates &state) {

}

/**
 * 2Dレンダリングのデフォルトステートを取得する
 */
glstates RenderState::default2D() {
    glstates states = { 0 };
    return states;
}

/**
 * 3Dレンダリングのデフォルトステートを取得する
 */
glstates RenderState::default3D() {
    glstates states = { 0 };
    states.flags |= (1 << GLStates_Cull_Back); // 背面カリング
    states.flags |= (1 << GLStates_DepthTest_Enable); // 深度テスト
    return states;
}

namespace {
}

/**
 * 現在のThreadに紐付いたStateを取得する
 */
std_shared_ptr<RenderState> RenderState::current() {

}

/**
 * 現在のスレッドで使用しなくなった
 */
void RenderState::unuseThisThread() {

}
}
