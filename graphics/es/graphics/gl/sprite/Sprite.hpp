#ifndef es_graphics_gl_SPRITE_HPP_
#define es_graphics_gl_SPRITE_HPP_

namespace es {

enum SpritePosition_e {
    /**
     * X方向中央
     */
    SpritePosition_CenterX = 0x1 << 0,

    /**
     * Y方向中央
     */
    SpritePosition_CenterY = 0x1 << 1,

    /**
     * 右寄せ
     */
    SpritePosition_Right = 0x1 << 2,

    /**
     * 下寄せ
     */
    SpritePosition_Bottom = 0x1 << 3,

    /**
     * 上寄せ
     */
    SpritePosition_Top = 0,

    /**
     * 左寄せ
     */
    SpritePosition_Left = 0,

    /**
     * 中央
     */
    SpritePosition_Center = SpritePosition_CenterX | SpritePosition_CenterY,
};

class Sprite: public Object {

    /**
     * レンダリング対象テクスチャ
     */
    MTexture texture;

    /**
     * レンダリング色
     */
    Color color;

    /**
     * 画像のソース位置
     */
    RectF srcArea;

    /**
     * 画像のレンダリング位置
     */
    RectF dstArea;

    /**
     * スプライトの回転角
     */
    float rotate;

protected:
    virtual MTexture getRenderingTexture() const {
        return texture;
    }
public:
    Sprite(MTexture texture) {
        this->texture = texture;
        rotate = 0;
        color.rgba = 0xFFFFFFFF;

        srcArea.setXYWH(0, 0, texture->getWidth(), texture->getHeight());
        dstArea = srcArea;
    }

    /**
     *
     */
    const RectF& getSrc() const {
        return srcArea;
    }

    /**
     *
     */
    const RectF& getDst() const {
        return dstArea;
    }

    /**
     * 描画領域の幅を取得する。
     * @return
     */
    float getDstWidth() {
        return dstArea.width();
    }

    /**
     * 描画領域の高さを取得する。
     * @return
     */
    float getDstHeight() {
        return dstArea.height();
    }

    float getDstLeft() {
        return dstArea.left;
    }

    float getDstRight() {
        return dstArea.right;
    }

    float getDstTop() {
        return dstArea.top;
    }

    float getDstBottom() {
        return dstArea.bottom;
    }

    float getDstCenterX() {
        return dstArea.centerX();
    }

    float getDstCenterY() {
        return dstArea.centerY();
    }

    /**
     * 描画位置を設定する。
     *
     * @param x
     * @param y
     * @param scaleX
     * @param scaleY
     * @param flags
     */
    Sprite* setSpritePosition(float x, float y, float dstWidth, float dstHeight, uint SpritePosition_flags) {
        //! 横方向の補正を行う
        {
            if (SpritePosition_flags & SpritePosition_CenterX) {
                x -= (dstWidth / 2);
            } else if (SpritePosition_flags & SpritePosition_Right) {
                x -= dstWidth;
            }
        }

        //! 縦方向の補正を行う
        {
            if (SpritePosition_flags & SpritePosition_CenterY) {
                y -= (dstHeight / 2);
            } else if (SpritePosition_flags & SpritePosition_Bottom) {
                y -= (dstHeight);
            }
        }

        dstArea.setXYWH(x, y, dstWidth, dstHeight);

        return this;
    }

    /**
     *
     */
    Sprite* setSpritePosition(float x, float y, uint SpritePosition_flags) {
        return setSpritePosition(x, y, srcArea.width(), srcArea.height(), SpritePosition_flags);
    }

    /**
     * 指定したピクセル数、描画エリアを移動する。
     * @param x
     * @param y
     */
    Sprite* offsetSpritePosition(float x, float y) {
        dstArea.offset(x, y);
        return this;
    }

    /**
     * 描画色RGBAを設定する。
     *
     * @param color
     */
    Sprite* setColorRGBA(const Color color) {
        this->color = color;
        return this;
    }

    /**
     * 描画色RGBのみを設定する。
     * Aの値は保たれる。
     * @param colorRGBX
     */
    Sprite* setColorRGB(const Color colorRGBX) {
        uint8_t alpha = color.tag.a;
        color = colorRGBX;
        color.tag.a = alpha;
        return this;
    }

    /**
     * 色を指定した位置へ遷移させる
     * @param targetRGBA
     * @param offset
     */
    Sprite* moveColorRGBA(const Color targetRGBA, const uint8_t offset) {
        color = Color::moveColor(color, targetRGBA, offset);
        return this;
    }

    /**
     * αのみ遷移させる
     * RGBは固定される。
     * @param targetA
     * @param offset
     */
    Sprite* moveColorA(rgba32 targetA, uint offset) {
        moveColorRGBA(Color::fromRGBAi(color.tag.r, color.tag.g, color.tag.b, targetA), offset);
        return this;
    }

    /**
     * RGBのみ遷移させる。
     * αは固定される。
     * @param targetRGB
     * @param offset
     */
    Sprite* moveColorRGB(Color targetRGB, uint offset) {
        targetRGB.tag.a = color.tag.a;
        moveColorRGBA(targetRGB, offset);
        return this;
    }

    /**
     * 画像のαのみを変更する
     * @param alpha
     */
    Sprite* setColorA(uint alpha) {
        color.tag.a = alpha;
        return this;
    }

    /**
     * 画像のアルファのみを変更する。
     * @param alpha
     */
    Sprite* setColorA(float alpha) {
        setColorA((uint)(alpha * 255));
        return this;
    }

    /**
     * スプライトの描画色を取得する
     */
    Color getColor() const {
        return color;
    }

    /**
     * 描画色RGBAを設定する。 値は0.0f〜1.0fである必要がある。
     *
     * @param r
     * @param g
     * @param b
     * @param a
     */
    Sprite* setColorRGBA(const float r, const float g, const float b, const float a) {
        color = Color::fromRGBAf(r, g, b, a);
        return this;
    }

    /**
     *
     */
    Sprite* setColor(Color color) {
        this->color = color;
        return this;
    }

    /**
     * レンダリングを行う
     */
    virtual void rendering(MSpriteManager spriteManager) {
        spriteManager->renderingImage(getRenderingTexture(), srcArea.left, srcArea.top, srcArea.width(), srcArea.height(), dstArea.left, dstArea.top, dstArea.width(), dstArea.height(), rotate, color.rgba);
    }
};

typedef std_shared_ptr<Sprite> MSprite;

}

#endif /* SPRITE_HPP_ */
