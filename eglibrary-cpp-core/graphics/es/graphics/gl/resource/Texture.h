#ifndef es_graphics_gl_TEXTURE_H_
#define es_graphics_gl_TEXTURE_H_

#include    "es/Graphics.hpp"
#include    "es/graphics/PixelFormat.hpp"
#include    "es/graphics/gl/context/ShaderState.h"
#include    "es/graphics/gl/context/ThreadDevice.h"
#include    "es/graphics/gl/GLObject.h"

namespace es {

struct TextureLoadOption {
    /**
     * Decode Format
     */
    PixelFormat_e format;

    /**
     * trueの場合強制的にPowerOfTwo変換して読み込む
     */
    bool convert_pot;

    /**
     * trueの場合、mipmapを生成する
     */
    bool gen_mipmap;

    /**
     * キャンセルチェックを行う。
     * 読込中にtrueを検出した場合、その時点で読込を停止する
     */
    bool load_cancel;

    /**
     *
     */
    TextureLoadOption() {
        convert_pot = gen_mipmap = load_cancel = false;
        format = PixelFormat_NativeRGBA;
    }
};

class Texture: public GLObject {
    struct {
        /**
         * テクスチャサイズとしての幅
         */
        int16_t tex_width;
        /**
         * テクスチャサイズとしての高さ
         */
        int16_t tex_height;

        /**
         * 画像としての幅
         */
        int16_t img_width;

        /**
         * 画像としての高さ
         */
        int16_t img_height;
    } size;

    /**
     * テクスチャバインドターゲット
     *
     * default = GL_TEXTURE_2D
     */
    GLenum target;

    /**
     * テクスチャ領域を確保済みならtrue
     */
    bool allocated;

    /**
     * 状態管理
     */
    struct {
        /**
         * GL_TEXTURE_MAG_FILTER
         * default = GL_NEAREST
         */
        GLenum minFilter;

        /**
         * GL_TEXTURE_MIN_FILTER
         * default = GL_NEAREST
         */
        GLenum magFilter;

        /**
         * GL_TEXTURE_WRAP_S
         * default = GL_CLAMP_TO_EDGE
         */
        GLenum wrapS;

        /**
         * GL_TEXTURE_WRAP_T
         * default = GL_CLAMP_TO_EDGE
         */
        GLenum wrapT;
    } context;

    /**
     * gen texture
     */
    GLuint handle;
public:
    Texture();
    virtual ~Texture();

    /**
     * 画像としての幅を取得する。
     */
    virtual uint getWidth() const {
        return size.img_width;
    }

    /**
     * 画像としての高さを取得する
     */
    virtual uint getHeight() const {
        return size.img_height;
    }

    /**
     * 画像としてのアスペクト比を取得する
     */
    virtual float getAspect() const {
        return (float) getWidth() / (float) getHeight();
    }

    /**
     * テクスチャとしての幅を取得する。
     * POT変換を行う場合がある。
     */
    virtual uint getTextureWidth() const {
        return size.tex_width;
    }

    /**
     * テクスチャとしての高さを取得する。
     * POT変換を行う場合がある。
     */
    virtual uint getTextureHeight() const {
        return size.tex_height;
    }

    /**
     * テクスチャとしてのアスペクト比を取得する
     */
    virtual float getTextureAspect() const {
        return (float) getTextureWidth() / (float) getTextureHeight();
    }

    /**
     * NPOTテクスチャの場合trueを返す。
     * GPUに渡すステータスをチェックするため、getTextureWidth()とgetTextureHeight()をチェックする
     */
    virtual bool isPowerOfTwoTexture();

    /**
     * ラップモードを指定する
     */
    virtual void setWrapMode(const GLenum s, const GLenum t);

    /**
     * フィルタモードを指定する
     */
    virtual void setFilter(const GLenum min, const GLenum mag);

    /**
     * mipmapを自動生成する
     */
    virtual void genMipmaps();

    /**
     * glBindTexgture(target, xxx)の内容を変更する
     */
    virtual void setBindTarget(const GLenum target) {
        this->target = target;
    }

    /**
     * テクスチャピクセル用のメモリを確保する
     * @param pixelFormat 確保するピクセルのビット配置(RGB565、RGBA5551、RGB8、RGBA8、BGRA8）
     */
    virtual void allocPixelMemory(const PixelFormat_e pixelFormat, const int miplevel, MShaderState state);

    /**
     * 外部要因でallocを行った（拡張機能とか）場合に呼び出す
     */
    virtual void onAllocated();

    /**
     * バインドされたテクスチャユニットの番号を取得する。
     */
    virtual int getBindTextureUnitIndex(MShaderState state) const;

    /**
     * バインド済みである場合true
     */
    virtual bool isBinded(int *resultIndex, MShaderState state) const;

    /**
     * 番号を自動的に付与してバインドする
     *
     * @return バインドしたテクスチャユニット番号
     */
    virtual uint bind(MShaderState state);

    /**
     * テクスチャをindex番のユニットに関連付ける
     */
    virtual void bind(const uint index, MShaderState state);

    /**
     * バインドを解除する
     */
    virtual void unbind(MShaderState state);

    /**
     * テクスチャ操作ハンドルを取得する
     */
    virtual GLuint getHandle() const {
        return handle;
    }

    /**
     * 管理している資源を開放する
     */
    virtual void dispose();
};

typedef std_shared_ptr<Texture> MTexture;

}

#endif /* TEXTURE_H_ */
