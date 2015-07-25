#pragma once

#include    "es/Graphics.hpp"

namespace es {

enum GraphicsEngine_e {
    GraphicsEngine_OpenGL,
    GraphicsEngine_DirectX,
    GraphicsEngine_Metal,
    GraphicsEngine_Unknown,
};

/**
 * OpenGLのバージョンを示す
 * GL互換バージョン : XX
 * ES互換バージョン :   XX
 * GL(1) or ES(0) :     X
 */
enum OpenGLVersion_e {
    OpenGLVersion_ES20 = 30200,
    OpenGLVersion_ES30 = 40300,
    OpenGLVersion_ES31 = 45310,
    OpenGLVersion_41 = 41301,
    OpenGLVersion_Unknown = 0,
};


/**
 * OpenGL Shader Languageのバージョンを示す
 * GL互換バージョン : XXnnn
 * ES互換バージョン : nnXXn
 * ES対応ならば1   : nnnnX
 */
enum OpenGLSLVersion_e {
    OpenGLSLVersion_100 = 30201,
    OpenGLSLVersion_300es = 43301,
    OpenGLSLVersion_310es = 45311,
    OpenGLSLVersion_400 = 40300,
    OpenGLSLVersion_410 = 41300,
};

/**
 * 対応している拡張機能
 */
enum GPUExtension_e {
    /**
     * ETC1に対応している
     * GL_OES_compressed_ETC1_RGB8_texture
     */
            GPUExtension_Texture_ETC1,

    /**
     * PVRTC対応
     * GL_IMG_texture_compression_pvrtc
     */
            GPUExtension_Texture_PVRTC,

    /**
     * S3TC対応
     * GL_EXT_texture_compression_s3tc
     */
            GPUExtension_Texture_S3TC,

    /**
     * BGRAテクスチャ対応
     * GL_EXT_texture_format_BGRA8888
     */
            GPUExtension_Texture_BGRA8888,

    /**
     * external画像
     * GL_OES_EGL_image_external
     * for Android（support SurfaceTexture）
     */
            GPUExtension_OES_EGL_image_external,

    /**
     * 深度テクスチャが利用できる
     * GL_OES_depth_texture
     */
            GPUExtension_Texture_Depth,

    /**
     * タイルレンダリング対応
     * GL_EXT_discard_framebuffer
     * PowerVR(iOS, GalaxyNexus, etc...)
     * Mali(Nexus 10, Galaxy S2, etc...)
     */
            GPUExtension_TileBasedDeferredRendering,

    /**
     * half floatテクスチャが利用できる
     */
            GPUExtension_Texture_HalfFloat,

    /**
     * レンダリングバッファにRGB8/RGBA8を利用する
     * 利用ができなかった場合、RGB565/RGB5_A1に丸められる
     */
            GPUExtension_Renderbuffer_RGB8_RGBA8,

    /**
     * レンダリングバッファの深度ステンシルでD24bit/S8bitのパックが可能
     * 利用ができなかった場合、各々に確保される
     */
            GPUExtension_Renderbuffer_PackedDepth24Stencil8,

    /**
     * レンダリングバッファのD24bitが利用可能
     * 利用ができなかった場合、16bitで確保される
     */
            GPUExtension_Renderbuffer_Depth24,

    /**
     * レンダリングバッファのD32bitが利用可能
     * 利用ができなかった場合、16bitで確保される
     */
            GPUExtension_Renderbuffer_Depth32,

    /**
     * VertexArrayObjectをサポートしている。
     * GLES30 / GL40以降は標準サポート
     */
            GPUExtension_VertexArrayObject,

    /**
     * num flags
     */
            GPUExtension_Num,
};

/**
 * サポートする代表的な系列機を列挙する
 */
enum GPUFamily_e {
    /**
     * Mali GPU
     * GalaxyS2, GalaxyS3, Nexus10
     */
            GPUFamily_Mali,

    /**
     * PowerVR GPU
     * iOS, GalaxyNexus
     */
            GPUFamily_PowerVR,

    /**
     * Adreno GPU
     * XperiaGX
     */
            GPUFamily_Adreno,

    /**
     * Tegra GPU
     * OptimusPad, GalaxyTab10.1, Nexus7
     */
            GPUFamily_Tegra,

    /**
     * GeForce Desktop
     * MacBook Pro
     */
            GPUFamily_GeForce,

    /**
     * Intel Desktop
     * MacBook Air
     */
            GPUFamily_IntelHdGraphics,

    /**
     * not found...
     */
            GPUFamily_Unknown,
};

/**
 * GPUのステータスを格納する。
 * 実行環境で変動するステータスを格納する。
 */
class GPU {
public:

    /**
     * レンダラー名を取得する
     */
    static const string &getRenderer();

    /**
     * ベンダー名を取得する
     */
    static const string &getVendor();

    /**
     * バージョン名を取得する
     */
    static const string &getVersion();

    /**
     * GLSLのバージョン名を取得する
     */
    static const string &getGlslVersion();

    /**
     * OpenGLのバージョンを取得する
     */
    static const OpenGLVersion_e getGLVersion();

    /**
     * OpenGL ESであればtrue
     */
    static const bool isOpenGLES();

    /**
     * 頂点属性の最大数を取得する。
     */
    static uint32_t getMaxVertexAttributes();

    /**
     * テクスチャユニットの最大数を取得する
     * この枚数を超えるテクスチャ処理は行えない。
     */
    static uint32_t getMaxTextureUnits();

    /**
     * テクスチャの一辺のサイズの最大値
     */
    static uint32_t getMaxTextureSize();

    /**
     * 頂点シェーダのUniformベクトル最大数を取得する
     */
    static uint32_t getMaxUniformVectorsVs();

    /**
     * フラグメントシェーダでのUniformベクトル最大数を取得する
     */
    static uint32_t getMaxUniformVectorsFs();

    /**
     * GPU拡張機能をサポートするかを調べる
     */
    static bool isSupport(const GPUExtension_e extension);

    /**
     * GPU系列のチェックを行う
     * 代表的な系列機以外はUnknownになる（エミュレーター等）
     */
    static GPUFamily_e getGPUFamily();

    /**
     * 実行されているGPUが同一系統のGPUである場合、trueを返す
     */
    static bool isFamily(const GPUFamily_e family);

private:
    friend class DeviceContext;

    GPU() = delete;

    ~GPU() = delete;

    /**
     * 初期化を行う
     */
    static void initialize();
};

}
