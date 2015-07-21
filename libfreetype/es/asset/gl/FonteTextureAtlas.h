#pragma once

#include "es/eglibrary.hpp"
#include "es/graphics/gl/resource/Texture.h"
#include "es/asset/FontFace.h"
#include <vector>

namespace es {

class FontTextureAtlas : public Object {
public:
    FontTextureAtlas(const std::shared_ptr<FontFace> font);

    virtual ~FontTextureAtlas() = default;

    class FontArea;

    /**
     * テキストを焼きこむ
     *
     * 新規に焼きこまれたテクスチャ数を返却する
     */
    uint bake(const std::wstring text);

    /**
     * 管理しているテクスチャリストを取得する
     */
    const std::vector<std::shared_ptr<Texture> > &getTextures() const;

    /**
     * エリアをピックアップする。
     * bakeされていない場合、nullptrを返す。
     */
    const std::shared_ptr<FontArea> pick(const wchar_t charactor) const;

    class FontArea {
    public:
        uint16_t getIndex() const;

        std::shared_ptr<FontCharactor> getCharactor() const;

        const RectI16 &getArea() const;

    private:
        /**
         * テクスチャのインデックス
         */
        uint16_t index;

        /**
         * 字体情報
         */
        std::shared_ptr<FontCharactor> charactor;

        /**
         * フォントBitmapが格納されている位置
         * ピクセル単位座標
         */
        RectI16 area;

        friend class FontTextureAtlas;
    };

private:
    /**
     * 管理しているテクスチャ一覧
     */
    std::vector<sp<Texture> > textures;
//    std::vector< <std::shared_ptr<Texture> >    textures;

    struct {
        /**
         * 次にテクスチャを焼き付けるX座標
         */
        uint16_t x = 0;

        /**
         * 次にテクスチャを焼き付けるY座標
         */
        uint16_t y = 0;

        /**
         * 現在の焼き込みラインの一番の高さ
         */
        uint16_t lineHeight = 0;
    } current;

    /**
     * 座標マップ
     */
    std::map<wchar_t, sp<Texture> > atlasMap;

    /**
     * フォント情報
     */
    sp<FontFace> font;
};


}
