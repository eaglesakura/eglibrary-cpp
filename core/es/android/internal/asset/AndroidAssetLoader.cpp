#ifdef __ANDROID__

#include    "AndroidAssetLoader.h"
#include    "AndroidMappedAsset.hpp"
#include    <android/asset_manager_jni.h>

namespace es {
namespace internal {

AndroidAssetLoader::AndroidAssetLoader(::jc::lang::object_wrapper jAssetManager, const std::string &newBasePath) :
        basePath(newBasePath) {
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