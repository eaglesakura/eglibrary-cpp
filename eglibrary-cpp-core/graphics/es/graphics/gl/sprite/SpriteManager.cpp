#include    "SpriteManager.h"

namespace es {

SpriteManager::SpriteManager() {
    quad.reset(new Quad());
}

SpriteManager::~SpriteManager() {
    this->dispose();
}

/**
 * シェーダーの設定と初期化を行う
 */
void SpriteManager::setShader(MShaderProgram shader) {
    this->shader = shader;
    this->shader->bind();

    // uniformを設定する
    {
        uniform.poly_data.setLocation(shader, "poly_data");
        uniform.poly_uv.setLocation(shader, "poly_uv");
        uniform.texture.setLocation(shader, "tex");
        uniform.color.setLocation(shader, "blendColor");
        uniform.aspect.setLocation(shader, "aspect");
        uniform.rotate.setLocation(shader, "rotate");
        uniform.texture_matrix.setLocation(shader, "unif_texm");
        uniform.colorOnly.setLocation(shader, "colorOnly");
        if (uniform.texture_matrix.valid()) {
            // テクスチャ行列が有効なら単位行列をデフォルト指定する
            setTextureMatrix(Matrix4x4());
        }
    }
    // attrを設定する
    {
        quad->setPositionAttribute(QuadPositionAttribute(shader, "vPosition"));
        quad->setCoordAttribute(QuadCoordAttribute(shader, "vTexCoord"));
    }
}

/**
 * テクスチャ用行列を設定する
 */
void SpriteManager::setTextureMatrix(const Matrix4x4 &m) {
    uniform.texture_matrix.upload(m);
}

/**
 * サーフェイスのアスペクト比を設定する
 */
float SpriteManager::getSurfaceAspect() {
    return display->getVirtualDisplayAspect();
}

/**
 * レンダリングやsetter等、何らかの操作をする場合、バインドを行っておくこと。
 */
void SpriteManager::bind() {
    context = DeviceContext::current();
    if (!display && context->getSurfaceSize().length() > 0) {
        display.reset(new VirtualDisplay());
        const Vector2i16 &surfaceSize = context->getSurfaceSize();
        display->setRealDisplaySize(surfaceSize.x, surfaceSize.y);
        display->setVirtualDisplaySize(surfaceSize.x, surfaceSize.y);
        display->updateViewport(VirtualDisplay::FitType_Auto);
    }
    MRenderState state = context->getRenderState();
    {
        const RectF &vp = display->getDisplayViewport();
        state->viewport((int) vp.left, (int) vp.top, (int)vp.width(), (int)vp.height());
    }

    if (shader) {
        shader->bind();
    }
    if (quad) {
        quad->bind();
    }
    assert_gl();
}

/**
 * レンダリング終了を宣言する
 */
void SpriteManager::unbind() {
    if (context) {
        context.reset();
    }
}

/**
 * 現在の環境にしたがってレンダリングさせる。
 */
void SpriteManager::rendering(const float x, const float y, const float width, const float height) {

    {
        // ポリゴンのXYWH情報を生成する
        const Vector2f &vDisplaySize = display->getVirtualDisplaySize();

        const float sizeX = width / vDisplaySize.x * 2;
        const float sizeY = height / vDisplaySize.y * 2;
        const float sx = x / vDisplaySize.x * 2;
        const float sy = y / vDisplaySize.y * 2;
        const float translateX = -1.0f + sizeX / 2.0f + sx;
        const float translateY = 1.0f - sizeY / 2.0f - sy;
        // データを転送する
        uniform.poly_data.upload(translateX, translateY, sizeX, sizeY);

//        eslog("translateX(%f) translateY(%f) sizeX(%f) sizeY(%f)", translateX, translateY, sizeX, sizeY);
    }

// レンダリングを行う
    quad->rendering();
}

/**
 * レンダリングを行う
 */
void SpriteManager::renderingImage(MTexture image, const float srcX, const float srcY, const float srcW, const float srcH, const float dstX, const float dstY, const float dstWidth, const float dstHeight, const float degree, const rgba32 rgba) {
// テクスチャを転送する
    if (image) {
        uniform.texture.upload(image, context);
    }
// ブレンド色を設定する
    uniform.color.upload(rgba);
// ポリゴン回転を設定する
    uniform.rotate.upload(es::degree2radian(degree));
// アスペクト比を転送する
    uniform.aspect.upload(getSurfaceAspect());

    if (image) {
//! テクスチャ描画位置を行列で操作する
        const float TEXTURE_WIDTH = (float) image->getTextureWidth();
        const float TEXTURE_HEIGHT = (float) image->getTextureHeight();

        const float sizeX = (float) es::round(srcW) / TEXTURE_WIDTH;
        const float sizeY = (float) es::round(srcH) / TEXTURE_HEIGHT;
        const float sx = (float) es::round(srcX) / TEXTURE_WIDTH;
        const float sy = (float) es::round(srcY) / TEXTURE_HEIGHT;

        uniform.poly_uv.upload(sx, sy, sizeX, sizeY);
        uniform.colorOnly.upload(false);
    } else {
        // UV値を0に設定して、テクスチャキャッシュを使わせる
//        uniform.poly_uv.upload(0, 0, 0, 0);
        uniform.colorOnly.upload(true);
    }
    assert_gl();

    this->rendering(es::round(dstX), es::round(dstY), (int32_t) dstWidth, (int32_t) dstHeight);
}

/**
 * 四角形描画を行う
 */
void SpriteManager::renderingRect(const float x, const float y, const float w, const float h, const rgba32 rgba) {
    renderingImage(MTexture(), 0, 0, 0, 0, x, y, w, h, 0.0f, rgba);
}

/**
 *
 */
void SpriteManager::dispose() {
    quad.reset();
    shader.reset();
    display.reset();
}

}
