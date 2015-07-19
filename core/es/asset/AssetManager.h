#pragma once

#include <es/system/Object.hpp>
#include <memory>
#include <list>

namespace es {
class IAsset;

class IAssetLoader;

class AssetManager : public Object {
public:
    AssetManager() = default;

    virtual ~AssetManager() = default;

    /**
     * カスタムローダーを追加する
     *
     * 後から追加されたものが優先的に利用される。
     */
    void addLoader(const std::shared_ptr<IAssetLoader> loader);

    /**
     * カスタムローダーを削除する
     */
    void removeLoader(const std::shared_ptr<IAssetLoader> loader);

    /**
     * アセットの読み込みを行う
     */
    virtual std::shared_ptr<IAsset> load(const std::string &path);

private:
    std::list<std::shared_ptr<IAssetLoader> > loaders;
};

}


