#include "es/OpenGL.hpp"
#include "UnitTestDeviceContext.h"

namespace es {
namespace debug {

UnitTestDeviceContext::UnitTestDeviceContext() {
    DeviceContext::current();
    assert(DeviceContext::getContextNum() == 1);
}

UnitTestDeviceContext::~UnitTestDeviceContext() {
    DeviceContext::unuseThisThread();
    assert(DeviceContext::getContextNum() == 0);
}
}
}