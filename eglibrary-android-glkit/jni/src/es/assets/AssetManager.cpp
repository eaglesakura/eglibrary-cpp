#include    "AssetManager.h"

#include    <android/asset_manager_jni.h>

namespace es {

AssetManager::AssetManager(::jc::lang::object_wrapper jAssetManager) {
    assert(jAssetManager.getJobject());

    this->jAssetManager = jAssetManager;
    this->assets = AAssetManager_fromJava(jAssetManager.getEnv(), jAssetManager.getJobject());
    assert(this->assets);
}

AssetManager::~AssetManager() {
}

/**
 * アセットを読み出し、メモリへコピーする。
 *
 * 読み込みに失敗した場合、nullptrを返却する。
 */
ByteBuffer AssetManager::load(const char* path) {
    AAsset *pAsset = AAssetManager_open(assets, path, AASSET_MODE_BUFFER);

    if (pAsset == nullptr) {
        eslog("load error(%s)", path);
        return ByteBuffer();
    }
    uint bufferSize = AAsset_getLength(pAsset);
    assert(bufferSize);

    eslog("load completed(%d bytes / %s)", bufferSize, path);

    // 戻り値を生成する
    ByteBuffer result = Buffer::create(bufferSize);

    // コピーする
    {
        const void* p = AAsset_getBuffer(pAsset);
        assert(p);

        memcpy(result.get(), p, bufferSize);
    }

    return result;
}
}
