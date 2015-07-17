#include "LocalFileAssetLoader.h"
#include "es/asset/internal/InMemoryAsset.hpp"

namespace es {


LocalFileAssetLoader::LocalFileAssetLoader(const std::string &_basePath) : basePath(_basePath) {
}

std::shared_ptr<IAsset> LocalFileAssetLoader::load(const std::string &path) {
    std::ifstream stream(basePath + path, std::ifstream::in | std::ifstream::binary);
    if (stream.fail()) {
//        eslog("LoadFiled(%s)", (basePath + path).c_str());
        return std::shared_ptr<IAsset>();
    }

    return std::shared_ptr<IAsset>(new internal::InMemoryAsset(stream));
}

}