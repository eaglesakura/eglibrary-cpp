#pragma once

#include "IAsset.hpp"
#include "IAssetLoader.hpp"

namespace es {

/**
 * ローカルファイル(SDカードやSSD)を読み込む
 */
class LocalFileAssetLoader : public Object, public IAssetLoader {
public:
    LocalFileAssetLoader(const std::string &_basePath = "");

    virtual ~LocalFileAssetLoader() = default;

    /**
     * アセットの読み込みを行う
     * 読み込みに失敗したらnullptrを返却する
     */
    virtual std::shared_ptr<IAsset> load(const std::string &path);

private:
    std::string basePath;
};

}


