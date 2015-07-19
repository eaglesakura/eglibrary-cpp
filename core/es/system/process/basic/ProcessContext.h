#pragma once

#include "es/system/process/IProcessContext.h"

#if defined(BUILD_MacOSX)

namespace es {

class ProcessContext : public IProcessContext {
public:
    virtual ~ProcessContext();

    /**
     * 実行されているプラットフォームを取得する
     */
    virtual PlatformType_e getPlatform();

    /**
     * プロセス起動時に呼び出す
     */
    static void onBootProcess();

private:
    ProcessContext();
};

}

#endif