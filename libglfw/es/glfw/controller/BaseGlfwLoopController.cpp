#include "es/system/NanoTimer.h"
#include "BaseGlfwLoopController.h"

#include "es/system/Thread.hpp"

#include "es/internal/protoground-internal.hpp"

namespace es {


BaseGlfwLoopController::BaseGlfwLoopController(std::shared_ptr<GlfwDevice> windowDevice) :
        device(windowDevice) {

}

void BaseGlfwLoopController::execute() {
    device->bind();

    // 初期化
    onLoopInitialize();

    Vector2i16 oldSurfaceSize;
    bool oldWindowVisible = false;

    const int SLEEP_TIME = 1000 / 120;   // 何らかの原因でsleepさせる場合の休止時間

    while (true) {
        device->pollEvents();
        if (device->isWindowClose()) {
            break;
        }

        // 可視状態チェック
        bool nowWindowVisible = device->isWindowVisible();
        if (nowWindowVisible != oldWindowVisible) {
            // 可視状態が変更された
            oldWindowVisible = nowWindowVisible;

            if (nowWindowVisible) {
                eslog("onResume(%x)", this);
                loopState = LoopState::Run;
                device->bind();
                onLoopResume();
            } else {
                eslog("onPause(%x)", this);
                loopState = LoopState::Pause;
                onLoopPaused();
                device->unbind();
            }
        }

        if (loopState == LoopState::Run) {
            // サーフェイスサイズチェック
            Vector2i16 nowSurfaceSize = device->getFramebufferSize();
            if (nowSurfaceSize != oldSurfaceSize) {
                eslog("onSurfaceChanged(%x) old(%dx%d) -> new(%dx%d)", this,
                      (int) oldSurfaceSize.x, (int) oldSurfaceSize.y,
                      (int) nowSurfaceSize.x, (int) nowSurfaceSize.y
                );
                oldSurfaceSize = nowSurfaceSize;

                onLoopSurfaceSizeChanged(oldSurfaceSize.x, oldSurfaceSize.y, nowSurfaceSize.x, nowSurfaceSize.y);
            }

            onLoopFrame();
        } else {
            NanoTimer::sleepMs(SLEEP_TIME);
        }
    }

    // 解放処理を行う
    {
        device->bind();
        eslog("onLoopFinish(%x)", this);
        onLoopFinish();
        device->unbind();
    }
}
}