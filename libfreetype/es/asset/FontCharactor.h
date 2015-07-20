#pragma once

#include "es/eglibrary.hpp"
#include <es/math/Vector2.hpp>

namespace es {

/**
 * フォントの1文字の情報を管理する。
 *
 * 文字の外形情報はTexture等に転送されるため、ここでは管理しない。
 */
class FontCharactor : public Object {
public:
    /**
     * アセントピクセル数を取得する
     * acent + decentのピクセル数は、sizeを上回る可能性がある。
     */
    int getAcent() const;

    /**
     * ディセントピクセル数を取得する
     * acent + decentのピクセル数は、sizeを上回る可能性がある。
     */
    int getDescent() const;

    /**
     * 文字自体のサイズを取得する。
     */
    const Vector2i16 &getBitmapSize() const;

    /**
     * 文字が正常に読み込まれていない（豆腐文字）の場合にtrueを返却する
     */
    virtual bool isTofu() const = 0;

    virtual ~FontCharactor() = default;

protected:
    FontCharactor() = default;

    /**
     * サイズ
     */
    Vector2i16 bitmapSize;

    /**
     * 管理されている文字
     */
    wchar_t charactor;

    int16_t acent = 0;
    int16_t descent = 0;
};

}