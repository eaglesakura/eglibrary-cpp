#pragma once

namespace es {
namespace debug {

/**
 * UnitTestでのデバイスコンテキスト管理をサポートする
 */
class UnitTestDeviceContext {
public:
    UnitTestDeviceContext();

    ~UnitTestDeviceContext();
};

#define ES_OPENGL_UNITTEST(...) ::es::debug::UnitTestDeviceContext   __private_device_context__

}
}
