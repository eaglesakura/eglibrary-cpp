#ifndef BASEAPPLICATION_H_
#define BASEAPPLICATION_H_

#include    "es/eglibrary.hpp"

namespace es {

class BaseApplication: public Object {
protected:
    BaseApplication();
public:
    virtual ~BaseApplication();

    virtual void onAppInitialize() = 0;
    virtual void onSurfaceChanged(int oldWidth, int oldHeight, int newWidth, int newHeight) = 0;
    virtual void onAppPause() = 0;
    virtual void onAppResume() = 0;
    virtual void onRequestRendering() = 0;
    virtual void onAppFrame() = 0;
    virtual void onAppFinalize() = 0;
};

}

#endif /* BASEAPPLICATION_H_ */
