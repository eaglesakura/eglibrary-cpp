#include <list>
#include "AssetManager.h"
#include "es/asset/IAsset.hpp"
#include "es/asset/IAssetLoader.hpp"

namespace es {

namespace {
static std::list<std::shared_ptr<IAssetLoader>> gLoaders;
}

void AssetManager::addLoader(const std::shared_ptr<IAssetLoader> loader) {
    gLoaders.push_front(loader);
}

void AssetManager::removeLoader(const std::shared_ptr<IAssetLoader> loader) {
    gLoaders.remove(loader);
}

std::shared_ptr<IAsset> AssetManager::load(const std::string &path) {

    for (std::shared_ptr<IAssetLoader> &loader : gLoaders) {
        auto result = loader->load(path);
        if (result) {
            return result;
        }
    }

    return std::shared_ptr<IAsset>();
}

}
