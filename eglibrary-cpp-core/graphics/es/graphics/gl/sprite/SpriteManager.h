#ifndef es_graphics_gl_SPRITEMANAGER_H_
#define es_graphics_gl_SPRITEMANAGER_H_

#include    "es/OpenGL.hpp"
#include    "es/Sprite.hpp"

namespace es {

/**
 * SpriteManager
 */
class SpriteManager: public Object {
protected:
    /**
     * レンダリング用固定シェーダ
     */
    MShaderProgram shader;

    /**
     * レンダリング中のみ有効なContext
     */
    MDeviceContext context;

    /**
     * 描画対象のディスプレイ
     */
    MVirtualDisplay display;

    struct {
        /**
         * 回転角
         */
        FloatUniform rotate;

        /**
         * アスペクト比
         */
        FloatUniform aspect;

        /**
         * ポリゴンデータ用
         */
        Vector4fUniofrm poly_data;

        /**
         * ポリゴンUV用
         */
        Vector4fUniofrm poly_uv;

        /**
         * ブレンド色
         */
        ColorUniform color;

        /**
         * テクスチャ
         */
        TextureUniform texture;

        /**
         * テクスチャ行列
         */
        Matrix4Uniform texture_matrix;

        /**
         * 色描画とテクスチャ描画の切り替え
         */
        BooleanUniform colorOnly;
    } uniform;

    /**
     * レンダリング用プリミティブ
     */
    MQuad quad;

    /**
     * 現在の環境にしたがってレンダリングさせる。
     */
    virtual void rendering(const float x, const float y, const float w, const float h);

public:
    SpriteManager();
    virtual ~SpriteManager();

    /**
     * シェーダーの設定と初期化を行う
     */
    virtual void setShader(MShaderProgram shader);

    /**
     * 仮想ディスプレイのセットアップを行う
     */
    virtual void setVirtualDisplay(MVirtualDisplay display) {
        this->display = display;
    }

    /**
     * 描画対象の仮想ディスプレイを取得する
     */
    virtual MVirtualDisplay getVirtualDisplay() {
        return display;
    }

    virtual MShaderProgram getShader() {
        return shader;
    }

    /**
     * 四角形描画を行う
     */
    virtual void renderingRect(const float x, const float y, const float w, const float h, const rgba32 rgba);

    /**
     * 四角形描画を行う
     */
    virtual void renderingRect(const RectI &dst, const rgba32 rgba) {
        renderingRect(dst.left, dst.top, dst.width(), dst.height(), rgba);
    }

    /**
     * 四角形描画を行う
     */
    virtual void renderingRect(const RectF &dst, const rgba32 rgba) {
        renderingRect(dst.left, dst.top, dst.width(), dst.height(), rgba);
    }

    /**
     * 画像を描画する
     * @param src{XYWH} テクスチャ内の座標をpix単位で指定する
     * @param dst{XYWH} 描画先の座標をpix単位で指定する。
     * @param degree 画像の回転角（360度系）
     * @param rgba   画像の色補正（RGBA形式）
     */
    virtual void renderingImage(MTexture image, const float srcX, const float srcY, const float srcW, const float srcH, const float dstX, const float dstY, const float dstW, const float dstH, const float degree, const rgba32 rgba);

    /**
     * 画像を描画する
     * @param src{XYWH} テクスチャ内の座標をpix単位で指定する
     * @param dst{XYWH} 描画先の座標をpix単位で指定する。
     * @param degree 画像の回転角（360度系）
     * @param rgba   画像の色補正（RGBA形式）
     */
    virtual void renderingImage(MTexture image, const float srcX, const float srcY, const float srcW, const float srcH, const float dstX, const float dstY, const float dstW, const float dstH) {
        renderingImage(image, srcX, srcY, srcW, srcH, dstX, dstY, dstW, dstH, 0.0f, 0xFFFFFFFF);
    }

    /**
     * 画像を描画する
     */
    virtual void renderingImage(MTexture image, const float x, const float y, const float width, const float height, const Color color) {
        const uint IMG_WIDTH = image->getWidth();
        const uint IMG_HEIGHT = image->getHeight();
        renderingImage(image, 0, 0, IMG_WIDTH, IMG_HEIGHT, x, y, width, height, 0.0f, color.rgba);
    }

    /**
     * 画像を描画する
     */
    virtual void renderingImage(MTexture image, const float x, const float y, const float width, const float height) {
        const uint IMG_WIDTH = image->getWidth();
        const uint IMG_HEIGHT = image->getHeight();
        renderingImage(image, 0, 0, IMG_WIDTH, IMG_HEIGHT, x, y, width, height, 0.0f, 0xFFFFFFFF);
    }

    /**
     * 画像を描画する
     */
    virtual void renderingImage(MTexture image, const float x, const float y, const Color color) {
        const uint IMG_WIDTH = image->getWidth();
        const uint IMG_HEIGHT = image->getHeight();
        renderingImage(image, 0, 0, IMG_WIDTH, IMG_HEIGHT, x, y, IMG_WIDTH, IMG_HEIGHT, 0.0f, color.rgba);
    }

    /**
     * 画像を描画する
     */
    virtual void renderingImage(MTexture image, const float x, const float y) {
        const uint IMG_WIDTH = image->getWidth();
        const uint IMG_HEIGHT = image->getHeight();
        renderingImage(image, 0, 0, IMG_WIDTH, IMG_HEIGHT, x, y, IMG_WIDTH, IMG_HEIGHT, 0.0f, 0xFFFFFFFF);
    }

    /**
     * 画像を描画する
     */
    virtual void renderingImage(MTexture image, const RectI &dst) {
        renderingImage(image, 0, 0, image->getWidth(), image->getHeight(), dst.left, dst.top, dst.width(), dst.height(), 0, 0xFFFFFFFF);
    }

    /**
     * 画像を描画する
     */
    virtual void renderingImage(MTexture image, const RectF &dst) {
        renderingImage(image, 0, 0, image->getWidth(), image->getHeight(), dst.left, dst.top, dst.width(), dst.height(), 0, 0xFFFFFFFF);
    }

    /**
     * 画像を描画する
     */
    virtual void renderingImage(MTexture image, const RectI &dst, const uint color) {
        renderingImage(image, 0, 0, image->getWidth(), image->getHeight(), dst.left, dst.top, dst.width(), dst.height(), 0, color);
    }

    /**
     * 画像を描画する
     */
    virtual void renderingImage(MTexture image, const RectF &dst, const uint color) {
        renderingImage(image, 0, 0, image->getWidth(), image->getHeight(), dst.left, dst.top, dst.width(), dst.height(), 0, color);
    }
    /**
     * 画像を描画する
     */
    virtual void renderingImage(MTexture image, const RectF &dst, const float degree, const uint color) {
        renderingImage(image, 0, 0, image->getWidth(), image->getHeight(), dst.left, dst.top, dst.width(), dst.height(), degree, color);
    }

    /**
     * テクスチャ用行列を設定する
     */
    virtual void setTextureMatrix(const Matrix4x4 &m);

    /**
     * サーフェイスのアスペクト比を設定する
     */
    virtual float getSurfaceAspect();

    /**
     * レンダリング用の矩形を取得する
     */
    virtual MQuad getRenderingQuad() const {
        return quad;
    }

    /**
     * デバッグ用のエリアレンダリング（fill色で塗りつぶし後、line色で線を描く）を行う。
     */
    virtual void renderingArea(const RectF &area, const rgba32 fillColor, const rgba32 lineColor) {
        {
            beginQuadRendering();
            renderingRect(area, fillColor);
        }
        {
            RectF lineArea = area;
            beginLineRendering();
            setLineWidth(2.0f);
            lineArea.inset(1, 1);
            renderingRect(lineArea, lineColor);
        }
        beginQuadRendering();
    }

    /**
     * 線レンダリングの幅を取得する
     */
    virtual void setLineWidth(const float width) {
        assert(context);
        context->getRenderState()->lineWidth(width);
    }

    /**
     * 線描画を開始する
     */
    virtual void beginLineRendering() {
        quad->setPrimitiveType(GL_LINE_LOOP);
        setLineWidth(2.0f);
    }

    /**
     * 四角形描画を開始する
     */
    virtual void beginQuadRendering() {
        quad->setPrimitiveType(GL_TRIANGLE_FAN);
    }

    /**
     * レンダリングやsetter等、何らかの操作をする場合、バインドを行っておくこと。
     */
    virtual void bind();

    /**
     * レンダリング終了を宣言する
     */
    virtual void unbind();

    /**
     * dispose
     */
    virtual void dispose();
};

typedef std_shared_ptr<SpriteManager> MSpriteManager;

}

#endif /* SPRITEMANAGER_H_ */
