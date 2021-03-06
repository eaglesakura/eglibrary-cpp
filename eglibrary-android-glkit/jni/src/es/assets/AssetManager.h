/*
 * AssetManager.h
 *
 *  Created on: 2014/08/25
 *      Author: eaglesakura
 */

#ifndef es_assets_ASSETMANAGER_H_
#define es_assets_ASSETMANAGER_H_

#include    "es/eglibrary.hpp"
#include    "es/memory/Buffer.hpp"
#include    "JointConnector.hpp"
#include    <android/asset_manager.h>
#include <es/memory/SafeArray.hpp>

namespace es {

/**
 * メモリマッピングしたアセットを管理する
 */
class MappedAsset {
    AAsset *pAsset;

    AAssetManager *pAssetManager;
public:
    /**
     * アクセス可能なバッファ
     */
    unsafe_array<uint8_t> buffer;

    MappedAsset(AAssetManager *pAssetManager, AAsset *pAsset);

    virtual ~MappedAsset();
};

/**
 * Androidのassetsディレクトリを管理する
 */
class AssetManager {
    /**
     * Java側のAssetManagerクラス
     */
    ::jc::lang::object_wrapper jAssetManager;

    /**
     * Native Asset
     */
    AAssetManager *assets;
public:
    AssetManager(::jc::lang::object_wrapper jAssetManager);

    virtual ~AssetManager();

    /**
     * アセットを読み出し、メモリへコピーする。
     *
     * 読み込みに失敗した場合、nullptrを返却する。
     */
    ByteBuffer load(const char *path);

    /**
     * メモリにマッピングする
     *
     * resultのデストラクタで自動的にリソースは解放される。
     */
    std_shared_ptr<MappedAsset> mmap(const char *path);

    /**
     * テキストファイルを文字列として読み込む
     */
    ::std::string loadText(const char *path) {
        ByteBuffer buffer = load(path);
        if (buffer.empty()) {
            return std::string();
        }
        return std::string((const char *) buffer.get(), buffer.length());
    }
};

typedef std_shared_ptr<AssetManager> MAssetManager;

}

#endif /* ASSETMANAGER_H_ */
