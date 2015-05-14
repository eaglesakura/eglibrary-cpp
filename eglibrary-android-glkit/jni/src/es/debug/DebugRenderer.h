//
// Created by @eaglesakura on 15/05/11.
//

#ifndef ASSEMBLE_DEBUGRENDERER_H
#define ASSEMBLE_DEBUGRENDERER_H

#include <es/graphics/Color.hpp>
#include "es/eglibrary.hpp"
#include "es_debug_RealtimeDebugWindowBridge.h"

namespace es {
namespace debug {

class DebugRenderer {
    std_shared_ptr<es::debug::RealtimeDebugWindowBridge> debugWindow;
public:
    DebugRenderer(jc::lang::object_wrapper jRealtimeDebugWindowBridge);

    virtual ~DebugRenderer() { }

    /**
     * 表示時間と色を指定して描画
     */
    virtual void addMessage(std::string message, uint showTimeMs = 0, Color color = Color::white());
};

typedef std_shared_ptr<DebugRenderer> MDebugRenderer;


}
}


#endif //ASSEMBLE_DEBUGRENDERER_H
