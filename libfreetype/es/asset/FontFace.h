#pragma once

#include "es/eglibrary.hpp"
#include <es/system/Object.hpp>
#include <es/math/Vector2.hpp>
#include <es/asset/image/IImageDecodeListener.hpp>

namespace es {

class FontCharactor;

/**
 * フォント1文字の外形情報を扱う
 */
class FontFace : public Object {
public:
    /**
     * ピクセル単位のフォントサイズを取得する。
     *
     * 文字はこのサイズに収まるようにレンダリングされる。
     */
    const Vector2i16 &getSize();

    /**
     * フォントサイズを指定する
     *
     * レンダリングされる文字はこのwidth/heightに収まるように設定される。
     */
    virtual void setSize(const uint width, const uint height) = 0;

    /**
     * 指定のバッファに文字をレンダリングし、外径情報を返す。
     *
     * バッファがnullである場合はレンダリングを行わず、外形情報だけを返す。
     */
    virtual std::shared_ptr<FontCharactor> rendering(const wchar_t charactor, selection_ptr<IImageDecodeListener> listener) = 0;

    virtual ~FontFace() = default;

protected:

    FontFace() = default;

    /**
     * 現在のピクセル単位のフォントサイズ
     */
    Vector2i16 size;
};

}


