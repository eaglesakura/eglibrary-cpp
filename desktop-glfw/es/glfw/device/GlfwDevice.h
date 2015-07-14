#pragma once

#include "es/eglibrary.hpp"
#include <es/system/Object.hpp>
#include <memory>

class GLFWwindow;

namespace es {

/**
 * GLFWのレンダリングデバイスを管理する
 */
class GlfwDevice : public Object {
    static int existDevices;

    GLFWwindow *window = nullptr;

    GlfwDevice(GLFWwindow *window);

public:

    virtual ~GlfwDevice();

    /**
     * ウィンドウが閉じられている場合はtrue
     */
    bool isWindowClose() const;

    /**
     * 呼び出しを行ったThreadに関連付ける
     */
    void bind();

    /**
     * Threadの関連付けを解除する
     */
    void unbind();

    /**
     * 画面のリフレッシュを行う
     */
    void swapBuffers();

    /**
     * ウィンドウを閉じる
     */
    void requestCloseWindow();

    /**
     * ウィンドウに対するイベントハンドリングを行う
     */
    void pollEvents();

    /**
     * インスタンスを生成する
     */
    static std::shared_ptr<GlfwDevice> createInstance(
            const uint width, const uint height,
            const bool resizeable,
            const std::string &title,
            const std::shared_ptr<GlfwDevice> sharedContext = std::shared_ptr<GlfwDevice>()
    );

    /**
     * オフスクリーン用のデバイスを生成する
     */
    static std::shared_ptr<GlfwDevice> createOffscreenInstance(
            const uint width, const uint height,
            const std::shared_ptr<GlfwDevice> sharedContext = std::shared_ptr<GlfwDevice>()
    );
};

}

