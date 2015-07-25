#pragma once

#include "es/protoground.hpp"
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
     * ビットマップ自体のサイズを取得する。
     * テクスチャの指定等に利用する
     */
    const Vector2i16 &getBitmapSize() const;

    /**
     * レンダリング時のBitmapオフセット量を取得する
     */
    const Vector2i16 &getBitmapOffset() const;

    /**
     * 字を表現するために必要なXYサイズを取得する。
     * このサイズはBitmapと上下左右の空白も含む。
     */
    const Vector2i16 &getAdvanceSize() const;

    /**
     * BitmapのYオフセット量を取得する
     */
    const int16_t getBitmapBearingY() const;

    /**
     * 管理している文字を取得する
     */
    const wchar_t getCode() const;

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
     * レンダリングのオフセット値
     */
    Vector2i16 bitmapOffset;

    /**
     * 次に描画すべき位置へのオフセット量
     */
    Vector2i16 advance;

    /**
     * レンダリング時のベースライン位置
     */
    Vector2i16 baseline;

    /**
     * ベースラインからのオフセットピクセル数
     */
    int16_t bitmapBearingY = 0;

    /**
     * 管理されている文字
     */
    wchar_t charactor = 0;
};

}