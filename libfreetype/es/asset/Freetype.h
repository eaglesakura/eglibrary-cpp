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
    Freetype();

    virtual ~Freetype() = default;

    /**
     * フォント情報を新規に生成する
     */
    virtual std::shared_ptr<FontFace> load(std::shared_ptr<IAsset> asset);

private:
    class Impl;

    std::shared_ptr<Impl> impl;
};

}


