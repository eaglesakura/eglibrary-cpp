#ifdef __ANDROID__

#include "AndroidContextUtil.h"

namespace es {

namespace internal {

namespace {

static ::jc::lang::class_wrapper gContext_class;
static ::jc::lang::object_wrapper gContext;
static jmethodID Context_getAssets;

}

void AndroidContextUtil::initialize(JNIEnv *env, jobject jContext) {
    if (gContext_class.hasObject()) {
        return;
    }

    ::jc::jni::initJniSupport(env);
    gContext = ::jc::lang::object_wrapper(jContext, env, true);
    gContext.globalRef().multiThread(true);

    gContext_class = ::jc::lang::class_wrapper(gContext.getClass(), env);
    gContext_class.globalRef().setMultiThreadAccess(true);
    assert(gContext_class.hasObject());

    Context_getAssets = gContext_class.getMethod("getAssets", "()Landroid/content/res/AssetManager;", false);
    assert(Context_getAssets);
}

::jc::lang::object_wrapper AndroidContextUtil::getAssets() {
    JNIEnv *env = gContext_class.getEnv();
    return ::jc::lang::wrapFromVM<::jc::lang::object_wrapper, jobject>(env,
                                                                       env->CallObjectMethod(gContext.getJobject(), Context_getAssets));
}

}

}

#endif