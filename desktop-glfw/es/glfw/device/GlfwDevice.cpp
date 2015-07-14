#include "GlfwDevice.h"
#include <GLFW/glfw3.h>

namespace es {
int GlfwDevice::existDevices = 0;

GlfwDevice::GlfwDevice(GLFWwindow *newWindow) : window(newWindow) {
    ++existDevices;
}

GlfwDevice::~GlfwDevice() {

    if (window) {
        if(glfwGetCurrentContext() == window) {
            glfwMakeContextCurrent(nullptr);
        }
        glfwDestroyWindow(window);
    }

    --existDevices;
    assert(existDevices >= 0);

    // 全てのデバイスが無くなったら解放する
    if (existDevices == 0) {
        glfwTerminate();
    }
}

bool GlfwDevice::isWindowClose() const {
    return glfwWindowShouldClose(window);
}

void GlfwDevice::swapBuffers() {
    glfwSwapBuffers(window);
}

std::shared_ptr<GlfwDevice> GlfwDevice::createInstance(const uint width, const uint height, const bool resizeable, const std::string &title, const std::shared_ptr<GlfwDevice> sharedContext) {

    std::shared_ptr<GlfwDevice> result;
    if (!sharedContext && !existDevices) {
        if (!glfwInit()) {
            return result;
        }
    }


    glfwSetErrorCallback([](int error, const char *msg) {
        eslog("GLFW Error(%x) Message(%s)", error, msg);
    });

    glfwWindowHint(GLFW_RESIZABLE, resizeable ? GL_TRUE : GL_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(width, height, title.c_str(), nullptr, sharedContext ? sharedContext->window : nullptr);
    if (!window) {
        return result;
    }

    result.reset(new GlfwDevice(window));

    glfwMakeContextCurrent(nullptr);
    return result;
}

void GlfwDevice::requestCloseWindow() {
    glfwSetWindowShouldClose(window, GL_FALSE);
}

void GlfwDevice::pollEvents() {
    glfwPollEvents();
}

void GlfwDevice::bind() {
    assert(!glfwGetCurrentContext());
    glfwMakeContextCurrent(window);
}

void GlfwDevice::unbind() {
    glfwMakeContextCurrent(nullptr);
}
}