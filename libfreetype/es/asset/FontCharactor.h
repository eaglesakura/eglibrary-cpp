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
     * 文字全体のサイズを取得する。
     * レイアウト等に利用する
     */
    const Vector2i16 &getFontSize() const;

    /**
     * レイアウト時に専有する領域を取得する
     *
     * @param withLeft 左側の空白領域を計算に含める場合はtrue
     * @param withRight 右側の空白領域を計算に含める場合はtrue
     */
    Vector2i16 getLayoutSize(bool withLeft, bool withRight) const;

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
     * ベースラインからのオフセットピクセル数
     */
    int16_t bitmapBearingY = 0;

    /**
     * フォント自体のサイズ
     */
    Vector2i16 fontSize;

    /**
     * フォント自体の幅ピクセル数
     */
    int16_t advanceWidth = 0;

    /**
     * 管理されている文字
     */
    wchar_t charactor = 0;
};

}