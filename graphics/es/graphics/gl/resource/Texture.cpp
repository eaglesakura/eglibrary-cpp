#include    "Texture.h"

namespace es {

Texture::Texture() {
    size.img_height = size.img_width = size.tex_height = size.tex_width = 0;
    context.magFilter = context.minFilter = context.wrapS = context.wrapT = 0;
    handle = 0;
    target = GL_TEXTURE_2D;
    allocated = false;

    // gen
    glGenTextures(1, &handle);
    assert_gl();
    assert(handle);
}

Texture::~Texture() {

}

/**
 * NPOTテクスチャの場合trueを返す。
 * GPUに渡すステータスをチェックするため、getTextureWidth()とgetTextureHeight()をチェックする
 */
bool Texture::isPowerOfTwoTexture() {
    return isPowerOfTwo(getTextureWidth()) && isPowerOfTwo(getTextureHeight());
}

/**
 * ラップモードを指定する
 */
void Texture::setWrapMode(const GLenum s, const GLenum t) {
    if (s != context.wrapS) {
        glTexParameteri(target, GL_TEXTURE_WRAP_S, s);
        context.wrapS = s;
    }
    if (t != context.wrapT) {
        glTexParameteri(target, GL_TEXTURE_WRAP_T, t);
        context.wrapT = t;
    }
}

/**
 * フィルタモードを指定する
 */
void Texture::setFilter(const GLenum min, const GLenum mag) {
    if (min != context.minFilter) {
        glTexParameteri(target, GL_TEXTURE_MIN_FILTER, min);
        context.minFilter = min;
    }
    if (mag != context.magFilter) {
        glTexParameteri(target, GL_TEXTURE_MAG_FILTER, mag);
        context.magFilter = mag;
    }
}

/**
 * mipmapを自動生成する
 */
void Texture::genMipmaps() {
    if (!isPowerOfTwoTexture()) {
        // mipmapが生成できなくても表示上は問題ないため、npotログだけ吐き出して終了
        eslog("texture is non power of two %d x %d", (int) size.tex_width, (int) size.tex_height);
        return;
    }
    glGenerateMipmap(target);
}

/**
 * テクスチャ用メモリを確保する
 */
void Texture::allocPixelMemory(const PixelFormat_e pixelFormat, const int miplevel, const int width, const int height) {
    if (getTextureWidth() == width && getTextureHeight() == height) {
        // リサイズが不要
        return;
    }

    const GLenum format = Pixel::toGLPixelFormat(pixelFormat);
    const GLenum type = Pixel::toGLPixelDataType(pixelFormat);
    glTexImage2D(GL_TEXTURE_2D, miplevel, format, width, height, 0, format, type, NULL);
    size.img_width = size.tex_width = width;
    size.img_height = size.tex_height = height;
    allocated = true;
}

/**
 * 外部要因でallocを行った（拡張機能とか）場合に呼び出す
 */
void Texture::onAllocated() {
    allocated = true;
}

/**
 * バインドされたテクスチャユニットの番号を取得する。
 */
int Texture::getBindTextureUnitIndex(MShaderState state) const {
    state = get_state(state);

    return state->getBindedTextureUnit(target, handle);
}

/**
 * バインド済みである場合true
 */
bool Texture::isBinded(int *resultIndex, MShaderState state) const {
    state = get_state(state);
    assert(state);

    const int bindUnit = getBindTextureUnitIndex(state);
    if (bindUnit < 0) {
        // テクスチャがバインドされていない
        return false;
    }

    if (resultIndex) {
        (*resultIndex) = bindUnit;
    }
    return true;
}

/**
 * 番号を自動的に付与してバインドする
 *
 * @return バインドしたテクスチャユニット番号
 */
uint Texture::bind(MDeviceContext context) {
    int index = get_state(context)->getFreeTextureUnitIndex(true);
    assert(index >= 0);
    bind((uint) index, context);

    return (uint) index;
}

/**
 * テクスチャをindex番のユニットに関連付ける
 */
void Texture::bind(const uint index, MDeviceContext context) {
    MShaderState state = get_state(context);

    if (state->isBindedTexture(index, target, handle)) {
        // 指定したIndexに既にバインドされていたら、activeだけを切り替えて何もしない
        state->activeTexture(index);
        return;
    } else {
        // 指定したIndexにバインドされていないため、activeとbindの両方を切り替える
        state->activeTexture(index);
        state->bindTexture(target, handle);
    }
}

/**
 * バインドを解除する
 */
void Texture::unbind(MDeviceContext context) {
    if (!handle) {
        return;
    }

    MShaderState state = get_state(context);

    state->unbindTexture(handle);
}

/**
 * 管理している資源を開放する
 */
void Texture::dispose() {
    if (handle) {
        MShaderState state = get_device()->getShaderState();
        assert(state);

        // 既存のステートをアンバインドする
        state->unbindTexture(handle);

        // 解放する
        glDeleteTextures(1, &handle);
        handle = 0;
    }
}

bool Texture::isAllocated() const {
    return allocated;
}

Object::QueryResult_e Texture::queryInterface(const int64_t interfaceId, void **resultInterfacePtr) const {
    if (interfaceId == InterfaceId_OpenGL_Texture) {
        return query_cast<Texture>(this, resultInterfacePtr);
    }
    return QueryResult_Failed;
}

void Texture::getImageArea(Vector2i16 *size, RectI16 *existArea) {
    if (size) {
        size->x = getTextureWidth();
        size->y = getTextureHeight();
    }
    if (existArea) {
        existArea->setXYWH(0, 0, (int16_t) getWidth(), (int16_t) getHeight());
    }
}
}
