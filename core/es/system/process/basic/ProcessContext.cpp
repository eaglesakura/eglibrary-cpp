#include "ProcessContext.h"

#if defined(BUILD_MacOSX)

#include "es/system/string/internal/Cpp11StringConverter.h"

namespace es {

namespace {
static std::shared_ptr<ProcessContext> gInstance;
}

ProcessContext::ProcessContext() {
    stringConverter.reset(new internal::Cpp11StringConverter());
}

ProcessContext::~ProcessContext() {

}

IProcessContext::PlatformType_e ProcessContext::getPlatform() {
    return PlatformType_OSX;
}

void ProcessContext::onBootProcess() {
    if (gInstance) {
        return;
    }

    gInstance.reset(new ProcessContext());
}

std::shared_ptr<IProcessContext> IProcessContext::getInstance() {
    if (!gInstance) {
        eslog("Call!! ProcessContext::onBootProcess()");
        assert(false);
    }
    return gInstance;
}

}
#endif