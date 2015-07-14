#include <list>
#include "TestAssetManager.h"
#include "es/asset/IAsset.hpp"
#include "es/asset/IAssetLoader.hpp"

namespace es {

namespace {
static std::list<std::shared_ptr<IAssetLoader>> gLoaders;
}

namespace debug {

void TestAssetManager::addLoader(const std::shared_ptr<IAssetLoader> loader) {
    gLoaders.push_front(loader);
}

void TestAssetManager::removeLoader(const std::shared_ptr<IAssetLoader> loader) {
    gLoaders.remove(loader);
}

std::shared_ptr<IAsset> TestAssetManager::load(const std::string &path) {

    for (std::shared_ptr<IAssetLoader> &loader : gLoaders) {
        auto result = loader->load(path);
        if (result) {
            return result;
        }
    }

    return std::shared_ptr<IAsset>();
}

}

}
