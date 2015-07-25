#pragma once

#include    "es/protoground.hpp"

namespace es {

class BaseApplication : public Object {
protected:
    BaseApplication() = default;

public:
    virtual ~BaseApplication() = default;

    virtual void onAppInitialize() = 0;

    virtual void onSurfaceChanged(int oldWidth, int oldHeight, int newWidth, int newHeight) = 0;

    virtual void onAppPause() = 0;

    virtual void onAppResume() = 0;

    virtual void onRequestRendering() = 0;

    virtual void onAppFrame() = 0;

    virtual void onAppFinalize() = 0;
};

}
