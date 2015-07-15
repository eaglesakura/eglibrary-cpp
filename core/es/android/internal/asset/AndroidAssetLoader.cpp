#ifdef __ANDROID__

#include    "AndroidAssetLoader.h"
#include    "AndroidMappedAsset.hpp"
#include    "es_glkit_GraphicAssets.h"

namespace es {
namespace internal {

namespace {


static ::jc::lang::class_wrapper jContextClass;
static jmethodID jContextMethod_getAssets;

}

AndroidAssetLoader::AndroidAssetLoader(::jc::lang::object_wrapper jApplicationContext, const std::string &newBasePath) :
        basePath(newBasePath) {

    if (!jContextClass.hasObject()) {
        JNIEnv *env = jApplicationContext.getEnv();
        jContextClass = ::jc::lang::class_wrapper(jApplicationContext.getClass(), env);
        jContextClass.globalRef().setMultiThreadAccess(true);
        assert(jContextClass.hasObject());


        jContextMethod_getAssets = jContextClass.getMethod("getAssets", "()Landroid/content/res/AndroidAssetManager;", false);
        assert(jContextMethod_getAssets);
    }

    this->jAssetManager = es::glkit::GraphicAssets::getAssets(jApplicationContext).globalRef();
    eslog("jAssetManager(%x)", this->jAssetManager.getJobject());

    this->assets = AAssetManager_fromJava(jAssetManager.getEnv(), jAssetManager.getJobject());
    assert(this->assets);
}

std::shared_ptr<IAsset> AndroidAssetLoader::load(const std::string &path) {
    AAsset *pAsset = AAssetManager_open(assets, (basePath + path).c_str(), AASSET_MODE_BUFFER);

    if (!pAsset) {
        eslog("load error(%s)", path.c_str());
        return ::std::shared_ptr<IAsset>();
    }

    return std::shared_ptr<IAsset>(new AndroidMappedAsset(assets, pAsset));
}

}
}

#endif