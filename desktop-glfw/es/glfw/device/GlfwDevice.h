#pragma once

#include "es/eglibrary.hpp"
#include <es/system/Object.hpp>
#include <memory>
#include "es/OpenGL.hpp"

// MacではGL3が優先されるため、旧バージョンのGLをガードする
#if !defined(__gl_h_) && defined(BUILD_MacOSX)
#define __gl_h_
#endif

#include <GLFW/glfw3.h>
#include <es/math/Vector2.hpp>

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
     * ウィンドウのサイズを取得する
     */
    Vector2i16 getWindowSize() const;

    /**
     * ウィンドウの可視状態を取得する
     */
    bool isWindowVisible() const;

    GLFWwindow *getWindow() const {
        return window;
    }

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

