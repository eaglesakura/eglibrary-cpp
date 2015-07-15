#ifdef __ANDROID__

#pragma once

#include    "es/eglibrary.hpp"
#include    "es/asset/IAssetLoader.hpp"
#include    "JointConnector.hpp"
#include    <jni.h>
#include    <android/asset_manager.h>
#include    <android/asset_manager_jni.h>


namespace es {

namespace internal {

/**
 * apkのassets内からデータを読み込む
 */
class AndroidAssetLoader : public IAssetLoader {
    /**
     * Java側のAssetManagerクラス
     */
    ::jc::lang::object_wrapper jAssetManager;

    /**
     * Native Asset
     */
    AAssetManager *assets;

    std::string basePath;
public:
    AndroidAssetLoader(::jc::lang::object_wrapper jContext, const std::string &newBasePath = "");

    virtual ~AndroidAssetLoader() = default;

    /**
     * アセットの読み込みを行う
     * 読み込みに失敗したらnullptrを返却する
     */
    virtual std::shared_ptr<IAsset> load(const std::string &path);
};

}
}

#endif /* __ANDROID__ */