#include "AndroidAssetManager.h"

#include    <android/asset_manager_jni.h>

namespace es {


MappedAsset::MappedAsset(AAssetManager *pAssetManager, AAsset *pAsset) {
    assert(pAssetManager);
    assert(pAsset);

    this->pAssetManager = pAssetManager;
    this->pAsset = pAsset;
}

MappedAsset::~MappedAsset() {
    AAsset_close(pAsset);
}

}

namespace es {
AndroidAssetManager::AndroidAssetManager(::jc::lang::object_wrapper jAssetManager) {
    assert(jAssetManager.getJobject());

    this->jAssetManager = jAssetManager;
    this->assets = AAssetManager_fromJava(jAssetManager.getEnv(), jAssetManager.getJobject());
    assert(this->assets);
}

AndroidAssetManager::~AndroidAssetManager() {
}

/**
 * アセットを読み出し、メモリへコピーする。
 *
 * 読み込みに失敗した場合、nullptrを返却する。
 */
ByteBuffer AndroidAssetManager::load(const char *path) {
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
        const void *p = AAsset_getBuffer(pAsset);
        assert(p);

        memcpy(result.get(), p, bufferSize);
    }

    AAsset_close(pAsset);

    return result;
}

std::shared_ptr<MappedAsset> AndroidAssetManager::mmap(const char *path) {
    AAsset *pAsset = AAssetManager_open(assets, path, AASSET_MODE_BUFFER);

    if (pAsset == nullptr) {
        eslog("load error(%s)", path);
        return std_shared_ptr<MappedAsset>();
    }
    uint bufferSize = AAsset_getLength(pAsset);
    assert(bufferSize);
    eslog("mapping completed(%d bytes / %s)", bufferSize, path);

    std_shared_ptr<MappedAsset> result(new MappedAsset(assets, pAsset));
    result->buffer.ptr = (uint8_t *) AAsset_getBuffer(pAsset);
    result->buffer.length = bufferSize;

    return result;
}
}
