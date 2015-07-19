#ifdef __ANDROID__

#pragma once

#include "es/system/process/IProcessContext.h"
#include "JointConnector.hpp"

namespace es {

class AndroidProcessContext : public IProcessContext {
public:
    virtual ~AndroidProcessContext();

    /**
     * JavaVMを取得する
     */
    JavaVM *getJavaVM() const;

    /**
     * 現在のThreadに紐付いたJNIEnvを取得する
     */
    JNIEnv *getThreadEnv() const;

    /**
     * Appliation情報を取得する
     */
    jc::lang::object_wrapper getApplicationContext() const;

    /**
     * 実行されているプラットフォームを取得する
     */
    virtual PlatformType_e getPlatform();

    /**
     * プロセス初期化を行う
     */
    static void onBootProcess(JavaVM *vm);

    /**
     * onCreate後に呼び出す。
     * ContextはApplicationContextである必要はなく、必ずgetApplication()で内部のContextを取得して利用する。
     */
    static void onCreateApplication(JNIEnv *env, jobject context);

private:
    class Impl;

    std::shared_ptr<Impl> impl;

    AndroidProcessContext();
};

}

#endif /* __ANDROID__ */
