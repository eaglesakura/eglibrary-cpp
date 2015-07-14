#pragma once

namespace es {
class IAsset;

class IAssetLoader;

class AssetManager {
    AssetManager() = delete;

    ~AssetManager() = delete;

public:
    /**
     * カスタムローダーを追加する
     *
     * 後から追加されたものが優先的に利用される。
     */
    static void addLoader(const std::shared_ptr<IAssetLoader> loader);

    /**
     * カスタムローダーを削除する
     */
    static void removeLoader(const std::shared_ptr<IAssetLoader> loader);

    /**
     * アセットの読み込みを行う
     */
    static std::shared_ptr<IAsset> load(const std::string &path);
};

}

