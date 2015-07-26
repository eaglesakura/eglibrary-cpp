#pragma once

#include "es/protoground.hpp"
#include "es/memory/SafeArray.hpp"
#include <es/asset/IAsset.hpp>
#include "es/asset/font/FontFace.h"
#include "es/asset/font/FontCharactor.h"

namespace es {

/**
 * Freetypeのフォントライブラリを管理する
 */
class Freetype : public Object {
public:
    struct FontLoadOption;

    Freetype();

    virtual ~Freetype() = default;

    /**
     * フォント情報を新規に生成する
     * フォントを利用中、IAssetのメモリ廃棄を行わないため、アセットがOpenし続けて問題のある仕様にしないこと。
     * openし続けたくない場合、util::toOnMemory()でアセットをオンメモリにラップすることで回避できる。
     */
    virtual std::shared_ptr<FontFace> load(std::shared_ptr<IAsset> asset, FontLoadOption *option);

    struct FontLoadOption {
    };
private:
    class Impl;

    std::shared_ptr<Impl> impl;
};

}


