#include <list>
#include "AssetManager.h"
#include "es/asset/IAsset.hpp"
#include "es/asset/IAssetLoader.hpp"

namespace es {

void AssetManager::addLoader(const std::shared_ptr<IAssetLoader> loader) {
    loaders.push_front(loader);
}

void AssetManager::removeLoader(const std::shared_ptr<IAssetLoader> loader) {
    loaders.remove(loader);
}

std::shared_ptr<IAsset> AssetManager::load(const std::string &path) {

    for (std::shared_ptr<IAssetLoader> &loader : loaders) {
        auto result = loader->load(path);
        if (result) {
            return result;
        }
    }

    eslog("LoadFiled(%s)", path.c_str());
    return std::shared_ptr<IAsset>();
}

}
