#pragma once

#include "estest/eglibrary-test.hpp"

namespace es {
namespace test {

/**
 * 適当なアセットを正常に読み込めることを検証する
 */
TEST(AssetLoaderTest, LoadTextFile) {
    std::shared_ptr<IAsset> asset = debug::TestAssetManager::load("shader/SpriteShader.vsh");
    ASSERT_TRUE((bool)asset);
    ASSERT_TRUE(asset->available());
//    eslog("TestShader\n%s", util::toString(asset).c_str());
}

}
}