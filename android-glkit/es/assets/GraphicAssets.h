#ifndef es_assets_GRAPHICSASSETS_H_
#define es_assets_GRAPHICSASSETS_H_

#include    "es/eglibrary.hpp"
#include    "es/memory/Buffer.hpp"
#include    "JointConnector.hpp"
#include    "AssetManager.h"

#include    "es/OpenGL.hpp"
#include    "es/Sprite.hpp"

#include    "es/assets/camera/CameraDeviceManager.h"

namespace es {

/**
 * テクスチャの種類を指定する
 */
enum TextureAssetType_e {
    /**
     * PNG/JPG等のAndroidがデフォルトで対応している形式である
     */
            TextureAssetType_AndroidImageFile,
};

/**
 * GL系リソースの読込を管理する
 */
class GraphicAssets : public Object {
    /**
     * Java側のAssetManagerクラス
     */
    ::jc::lang::object_wrapper jAssetManager;

    /**
     * Java側のApplicationContextクラス
     */
    ::jc::lang::object_wrapper jApplicationContext;

    /**
     * AssetManagerの管理
     */
    std::shared_ptr<AssetManager> assetManager;
public:
    /**
     *
     */
    GraphicAssets(::jc::lang::object_wrapper jApplicationContext);

    virtual ~GraphicAssets();

    /**
     * assets配下からテクスチャを読み出す
     */
    virtual MTexture loadTextureFromAssets(const std::string &path, const TextureAssetType_e type, const TextureLoadOption *option = nullptr);

    /**
     * assets配下からシェーダーを読み出す
     */
    virtual MShaderProgram loadShaderFromAssets(const std::string &vertShaderFile, const std::string &fragShaderFile);

    /**
     * SpriteManagerを生成する
     */
    virtual MSpriteManager createSpriteManager(const std::string &vertShaderFile, const std::string &fragShaderFile);

    /**
     * カメラを取得する
     */
    virtual MCameraDeviceManager connectCameraDevice(const CameraDeviceMode_e deviceType);
};

typedef std_shared_ptr<GraphicAssets> MGraphicAssets;

}

#endif /* GRAPHICSASSETS_H_ */
