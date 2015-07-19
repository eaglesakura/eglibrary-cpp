#include "IProcessContext.h"
#include "es/asset/AssetManager.h"

namespace es {

IProcessContext::IProcessContext() {
    assetManager.reset(new AssetManager());
}

std::shared_ptr<AssetManager> IProcessContext::getAssetManager() const {
    return assetManager;
}

std::shared_ptr<IStringConverter> IProcessContext::getStringConverter() const {
    return stringConverter;
}
}