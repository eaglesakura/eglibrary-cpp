#pragma once

#include "es/GLFWKit.hpp"

namespace es {

/**
 * GLFWを利用したループ処理のひな形
 */
class BaseGlfwLoopController : public Object {
public:
    virtual ~BaseGlfwLoopController() = default;

    /**
     * レンダリング用の無限ループを開始する
     */
    virtual void execute();

protected:
    enum LoopState {
        /**
         * 起動中
         */
                Booting,

        /**
         * 描画中
         */
                Run,

        /**
         * 停止中
         */
                Pause,


        /**
         * 廃棄を行う
         */
                Destroyed,
    } loopState = Booting;

    BaseGlfwLoopController(std::shared_ptr<GlfwDevice> windowDevice);

    /**
     * 処理の初期化を行わせる
     */
    virtual void onLoopInitialize() = 0;

    /**
     * 処理中にサーフェイスサイズが変更になったら呼び出される
     *
     * @param oldWidth
     * @param oldHeight
     * @param newWidth
     * @param newHeight
     */
    virtual void onLoopSurfaceSizeChanged(const uint oldWidth, const uint oldHeight, const uint newWidth, const uint newHeight) = 0;

    /**
     * 処理の一時停止を行う
     */
    virtual void onLoopPaused() = 0;

    /**
     * 処理の復帰を行う
     */
    virtual void onLoopResume() = 0;

    /**
     * 処理ループの１フレーム処理を行わせる
     */
    virtual void onLoopFrame() = 0;

    /**
     * 処理ループの終了を行わせる
     */
    virtual void onLoopFinish() = 0;

    /**
     * レンダリング対象のデバイス
     */
    std::shared_ptr<GlfwDevice> device;
};

}


