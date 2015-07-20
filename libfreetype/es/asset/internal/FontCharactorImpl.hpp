#pragma once

#include "es/asset/internal/FreetypeLibrary.hpp"
#include "es/asset/FontCharactor.h"
#include "es/asset/FontFace.h"

namespace es {
namespace internal {

class FontCharactorImpl : public FontCharactor {
    bool tofu = false;
public:
    /**
     * Face情報をコピーする。
     *
     */
    FontCharactorImpl(FontFace *parent, const FT_Face face) {
        this->bitmapSize.x = face->glyph->bitmap.width;
        this->bitmapSize.y = face->glyph->bitmap.pitch;


        if (bitmapSize.x == 0 || bitmapSize.y == 0) {
            // 正常に読み込めなかった場合は字形を補正して豆腐文字として扱う。
            this->acent = bitmapSize.y / 2;
            this->descent = bitmapSize.y / 2;
            bitmapSize = parent->getSize();
            tofu = true;
        } else {
            this->acent = face->size->metrics.ascender;
            this->descent = face->size->metrics.descender;
        }
    }

    virtual ~FontCharactorImpl() {
    }

    /**
     * 文字が正常に読み込まれていない（豆腐文字）の場合にtrueを返却する
     */
    virtual bool isTofu() const {
        return tofu;
    }

};

}
}