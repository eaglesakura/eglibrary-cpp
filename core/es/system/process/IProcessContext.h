#pragma once

#include "es/eglibrary.hpp"

namespace es {

class AssetManager;

class IStringConverter;

/**
 * プロセス情報を管理する
 */
class IProcessContext : public Object {
public:
    enum PlatformType_e {
        /**
         * Androidで実行されている
         */
                PlatformType_Android,

        /**
         * Mac OSXで実行されている
         */
                PlatformType_OSX,
    };

    virtual ~IProcessContext() = default;

    /**
     * アセット管理クラスを取得する
     */
    virtual std::shared_ptr<AssetManager> getAssetManager() const;

    /**
     * 文字列の変換クラスを取得する
     */
    virtual std::shared_ptr<IStringConverter> getStringConverter() const;

    /**
     * 実行されているプラットフォームを取得する
     */
    virtual PlatformType_e getPlatform() = 0;

    /**
     * 現在のプロセス情報を取得する
     */
    static std::shared_ptr<IProcessContext> getInstance();

protected:
    IProcessContext();

    std::shared_ptr<AssetManager> assetManager;

    std::shared_ptr<IStringConverter> stringConverter;
};

}