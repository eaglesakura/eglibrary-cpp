#include    "DeviceContext.h"
#include    <map>
#include    "es/graphics/GPU.h"
#include    "es/system/Thread.hpp"

namespace es {

DeviceContext::DeviceContext() {
    eslog("new ThreadDevice::ThreadDevice(%x)", this);

#if defined(BUILD_Android)
    {
        EGLDisplay display = eglGetCurrentDisplay();
        EGLSurface surface = eglGetCurrentSurface(EGL_READ);
        assert(surface);
        assert(display);
        EGLint temp = 0;
        eglQuerySurface(display, surface, EGL_WIDTH, &temp);
        surfaceSize.x = temp;
        eglQuerySurface(display, surface, EGL_HEIGHT, &temp);
        surfaceSize.y = temp;

        assert(eglGetError() == EGL_SUCCESS);
    }
#else
    {
        GLint xywh[4] = {0};
        glGetIntegerv(GL_VIEWPORT, xywh);
        assert_gl();
        surfaceSize.x = xywh[2];
        surfaceSize.y = xywh[3];
    }
#endif

    renderState.reset(new RenderState());
    renderState->set(createDefaultState2D());
    shaderState.reset(new ShaderState());

    eslog("current thread surface size(%d x %d)", surfaceSize.x, surfaceSize.y);
}

DeviceContext::~DeviceContext() {
    renderState->unbindResources();
    glFinish();
    eslog("delete ThreadDevice::~ThreadDevice(%x)", this);
}

/**
 * 2Dレンダリングのデフォルトステートを取得する
 */
glstates DeviceContext::createDefaultState2D() const {
    glstates states = {0};

    // viewport
    states.viewport.right = surfaceSize.x;
    states.viewport.bottom = surfaceSize.y;
    return states;
}

/**
 * 3Dレンダリングのデフォルトステートを取得する
 */
glstates DeviceContext::createDefaultState3D() const {
    glstates states = {0};
    states.flags |= (GLState_Cull_Back | GLState_DepthTest_Enable); // 背面カリング & 深度テスト

    // viewport
    states.viewport.right = surfaceSize.x;
    states.viewport.bottom = surfaceSize.y;
    return states;
}

namespace {

/**
 * グローバルなデバイス設定
 */
std::map<std::thread::id, MDeviceContext> g_devices;

/**
 * mutex
 */
mutex g_devicesMutex;

}

/**
 * 現在のThreadに関連付けられたデバイスを取得する
 */
::std::shared_ptr<DeviceContext> DeviceContext::current() {
    // 制御ロックをかける
    mutex_lock lock(g_devicesMutex);

    // デバイスがemptyならGPUCapsを初期化する
    if (g_devices.empty()) {
        GPU::getRenderer();
    }

    const auto currentThreadId = std::this_thread::get_id();
    auto itr = g_devices.find(currentThreadId);

    if (itr == g_devices.end()) {
        // 現在のThreadに対応するデバイスが無いなら新規生成する
        MDeviceContext result(new DeviceContext());
        g_devices.insert(std::make_pair(currentThreadId, result));

        return result;
    } else {
        // 対応するデバイスが見つかったらそれを返す
        return itr->second;
    }
}

/**
 * 現在のスレッドで使用しなくなった
 */
void DeviceContext::unuseThisThread() {
    // 制御ロックをかける
    mutex_lock lock(g_devicesMutex);

    auto itr = g_devices.find(std::this_thread::get_id());
    if (itr == g_devices.end()) {
        // 現在のThreadに対応するデバイスが無いなら何もしない
        return;
    } else {
        g_devices.erase(itr);
    }
}

uint DeviceContext::getContextNum() {
    // 制御ロックをかける
    mutex_lock lock(g_devicesMutex);

    return (uint) g_devices.size();
}
}
