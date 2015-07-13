#include "DebugRenderer.h"

namespace es {
namespace debug {

DebugRenderer::DebugRenderer(jc::lang::object_wrapper jGraphicsDebugRenderer) {
    this->debugWindow.reset(new es::debug::RealtimeDebugWindowBridge(jGraphicsDebugRenderer));
}

void DebugRenderer::addMessage(std::string message, uint showTimeMs, Color color) {
    JNIEnv *env = jc::jni::getThreadJniEnv();
    debugWindow->addTextMessage(
            jc::lang::string_wrapper(env->NewStringUTF(message.c_str()), env, false),
            es::debug::RealtimeDebugWindowBridge::FLAG_RENDERING_POSITION_LEFT,
            (int) showTimeMs,
            color.rgba
    );
}

}
}
