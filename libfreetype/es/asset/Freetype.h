#pragma once

#include "es/eglibrary.hpp"
#include "es/memory/SafeArray.hpp"
#include <es/asset/IAsset.hpp>
#include "es/asset/FontFace.h"
#include "es/asset/FontCharactor.h"

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
     */
    virtual std::shared_ptr<FontFace> load(std::shared_ptr<IAsset> asset, FontLoadOption *option);

    struct FontLoadOption {
        enum Flag_e {
            /**
             * フォント利用中アセットを常に保持し続ける
             * アセットの種類によっては大量のリソースをリークする恐れが有る
             */
                    Flag_AssetRetain = 0x1 << 0,

            /**
             * フォントファイルをメモリ内に複製する。
             * フォント精製後、アセットを解放しても問題ない。
             */
                    Flag_FontMemCopy = 0x0,
        };

        uint32_t flags = Flag_FontMemCopy;
    };
private:
    class Impl;

    std::shared_ptr<Impl> impl;
};

}


