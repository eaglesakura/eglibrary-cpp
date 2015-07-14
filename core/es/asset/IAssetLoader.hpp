#pragma once

#include <es/memory/Buffer.hpp>
#include "es/eglibrary.hpp"

namespace es {
class IAsset;

class IAssetLoader {
public:
    virtual ~IAssetLoader() = default;

    /**
     * アセットの読み込みを行う
     * 読み込みに失敗したらnullptrを返却する
     */
    virtual std::shared_ptr<IAsset> load(const std::string &path) = 0;
};

}