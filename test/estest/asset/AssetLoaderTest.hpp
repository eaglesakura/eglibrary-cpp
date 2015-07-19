#pragma once

#include "estest/eglibrary-test.hpp"

namespace es {
namespace test {

/**
 * 適当なアセットを正常に読み込めることを検証する
 */
TEST(AssetLoaderTest, LoadTextFile) {
    ASSERT_TRUE((bool) IProcessContext::getInstance());
    ASSERT_TRUE((bool) IProcessContext::getInstance()->getAssetManager());

    std::shared_ptr<IAsset> asset = IProcessContext::getInstance()->getAssetManager()->load("shader/SpriteShader.vsh");
    ASSERT_TRUE((bool) asset);
    ASSERT_TRUE(asset->available());
//    eslog("TestShader\n%s", util::toString(asset).c_str());
}

}
}