#include    "GPUCapacity.h"
#include    "es/memory/BitFlags.hpp"
#include    <vector>
#include    "es/util/StringUtil.h"

namespace es {

namespace {
static bool initialized = false;

/**
 * レンダラー名
 */
static std::string renderer;

/**
 * ベンダー名
 */
static std::string vendor;

/**
 * バージョン情報
 */
static std::string version;

/**
 * 保持しているエクステンション
 */
static std::vector<std::string> extensions;

/**
 * 頂点属性の最大数
 */
static uint32_t maxVertexAttrbs = 0;

/**
 * テクスチャユニットの最大数
 */
static uint32_t maxTextureUnits = 0;

/**
 * テクスチャの一辺の最大サイズ
 */
static uint32_t maxTextureSize = 0;

/**
 * 頂点シェーダでの最大Uniformベクトル数
 */
static uint32_t maxUniformVectorsVs = 0;

/**
 * フラグメントシェーダでの最大Uniformベクトル数
 */
static uint32_t maxUniformVectorsFs = 0;

/**
 * 拡張設定
 */
static BitFlags<GPUExtension_Num> extension_flags;

/**
 * GPUファミリー名
 */
static GPUFamily_e gpuFamily = GPUFamily_Unknown;

static OpenGLVersion_e glVersion = OpenGLVersion_Unknown;

}

/**
 * 初期化を行う
 */
void GPUCapacity::initialize() {
    if (initialized) {
        return;
    }

    initialized = true;

    {
        // TODO glGetStringは非推奨なので、将来的には別な方法に書き換える必要がある。
        // レンダラ名と系列機チェック
        const char *pRenderer = (const char *) glGetString(GL_RENDERER);
        renderer = pRenderer;

        struct GPUFamilyGroup {
            /**
             * チェックするGPU名
             */
            const char *name;

            /**
             * 登録されるファミリー
             */
            GPUFamily_e family;
        } gpu_groups[] = {
                {"PowerVR", GPUFamily_PowerVR},
                {"Mali",    GPUFamily_Mali},
                {"Tegra",   GPUFamily_Tegra},
                {"Adreno",  GPUFamily_Adreno},
                {"GeForce", GPUFamily_GeForce},
        };

        for (int i = 0; i < (sizeof(gpu_groups) / sizeof(GPUFamilyGroup)); ++i) {
            if (strstr(pRenderer, gpu_groups[i].name)) {
                // GPUファミリー
                gpuFamily = gpu_groups[i].family;
                eslog("GPU = %s(%s)", gpu_groups[i].name, pRenderer);
                break;
            }
        }

        if (gpuFamily == GPUFamily_Unknown) {
            eslog("GPU = Unknown(%s)", pRenderer);
        }
    }

    vendor = (const char *) glGetString(GL_VENDOR);
    version = (const char *) glGetString(GL_VERSION);

    // バージョンチェック
    {
        GLint major = 0;
        GLint minor = 0;
        glGetIntegerv(GL_MAJOR_VERSION, &major);
        glGetIntegerv(GL_MINOR_VERSION, &minor);
        if (glGetError() == GL_NO_ERROR) {
#if defined(BUILD_Android) || defined(BUILD_iOS)
#else
            if (major == 4 && minor == 1) {
                glVersion = OpenGLVersion_41;
            }
#endif
        }
    }
// エクステンション一覧を取得する
    {
        std::string extension_names;
        if (glVersion >= 300) {
            GLint numExtensions = 0;
            glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);
            for (int i = 0; i < numExtensions; ++i) {
                std::string ext((const char *) glGetStringi(GL_EXTENSIONS, i));
                if (extension_names.empty()) {
                    extension_names = ext;
                } else {
                    extension_names += std::string(" ") + ext;
                }
                extensions.push_back(ext);
            }
            assert_gl();

        } else {
            const char *pExtensions = (const char *) glGetString(GL_EXTENSIONS);
            StringUtils::split(pExtensions, " ", &extensions);
        }

        struct ExtensionFlag {
            /**
             * エクステンション名
             */
            const char *name;

            /**
             * 利用可能になる拡張フラグ
             */
            const GPUExtension_e extension;
        };

#define EXTENSION_NAME(def) #def

        const ExtensionFlag EXTENSIONS[] = {
                //  ETC1(Android)
                {EXTENSION_NAME(GL_OES_compressed_ETC1_RGB8_texture), GPUExtension_Texture_ETC1},
                //  PVRTC(PowerVR)
                {EXTENSION_NAME(GL_IMG_texture_compression_pvrtc),    GPUExtension_Texture_PVRTC},
                //  s3tc texture(Tegra)
                {"GL_EXT_texture_compression_s3tc",                   GPUExtension_Texture_S3TC},
                //  s3tc texture(Tegra)
                {EXTENSION_NAME(GL_COMPRESSED_RGBA_S3TC_DXT1_EXT),    GPUExtension_Texture_S3TC},
                //  s3tc texture(Tegra)
                {EXTENSION_NAME(GL_COMPRESSED_RGB_S3TC_DXT1_EXT),     GPUExtension_Texture_S3TC},
                //   BGRA(Android)
                {EXTENSION_NAME(GL_EXT_texture_format_BGRA8888),      GPUExtension_Texture_BGRA8888},
                //  SurfaceTexture(Android)
                {EXTENSION_NAME(GL_OES_EGL_image_external),           GPUExtension_OES_EGL_image_external},
                //  深度テクスチャ
                {EXTENSION_NAME(GL_OES_depth_texture),                GPUExtension_Texture_Depth},
                //  half float
                {EXTENSION_NAME(GL_OES_texture_half_float),           GPUExtension_Texture_HalfFloat},
                //  フレームバッファの無効化
                {EXTENSION_NAME(GL_EXT_discard_framebuffer),          GPUExtension_TileBasedDeferredRendering},
                //  レンダリングバッファにRGB/RGBA8が可能
                {EXTENSION_NAME(GL_OES_rgb8_rgba8),                   GPUExtension_Renderbuffer_RGB8_RGBA8},
                //  深度ステンシルパックが可能
                {EXTENSION_NAME(GL_OES_packed_depth_stencil),         GPUExtension_Renderbuffer_PackedDepth24Stencil8},
                //  深度ステンシルパックが可能
                {EXTENSION_NAME(GL_OES_depth24),                      GPUExtension_Renderbuffer_Depth24},
                //  深度ステンシルパックが可能
                {EXTENSION_NAME(GL_OES_depth32),                      GPUExtension_Renderbuffer_Depth32},
                //
        };

        // 対応している拡張機能を調べる
        for (int i = 0; i < (sizeof(EXTENSIONS) / sizeof(ExtensionFlag)); ++i) {
            // 拡張機能名がヒットした場合、フラグを有効にする
            if (strstr(extension_names.c_str(), EXTENSIONS[i].name)) {
                // check index
                extension_flags.enable(EXTENSIONS[i].extension);
                eslog("supported extension(%s)", EXTENSIONS[i].name);

                assert(GPUCapacity::isSupport(EXTENSIONS[i].extension));
            }
        }
#undef  EXTENSION_NAME

    }

    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, (GLint *) &maxVertexAttrbs);
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, (GLint *) &maxTextureUnits);
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, (GLint *) &maxTextureSize);
#ifdef GL_MAX_VERTEX_UNIFORM_VECTORS
    glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS, (GLint *) &maxUniformVectorsVs);
#else
    maxUniformVectorsVs = 250;
#endif

#ifdef GL_MAX_FRAGMENT_UNIFORM_VECTORS
    glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_VECTORS, (GLint *) &maxUniformVectorsFs);
#else
    maxUniformVectorsFs = 250;
#endif

    eslog("------------ GPU ------------");
    {
        eslog("GL_VERSION = %s", version.c_str());
        eslog("GL_VENDOR = %s", vendor.c_str());
        eslog("GL_RENDERER = %s", renderer.c_str());

// エクステンション一覧を出力する
        {
            std::vector<std::string>::iterator itr = extensions.begin(), end = extensions.end();

            while (itr != end) {
                if ((*itr).size()) {
                    eslog("GL_EXTENSIONS = %s", (*itr).c_str());
                    ++itr;
                } else {
                    itr = extensions.erase(itr);
                    end = extensions.end();
                }
            }
        }

        eslog("GL_MAX_VERTEX_ATTRIBS = %d", maxVertexAttrbs);
        eslog("GL_MAX_TEXTURE_IMAGE_UNITS = %d", maxTextureUnits);
        eslog("GL_MAX_TEXTURE_SIZE = %d", maxTextureSize);
        eslog("GL_MAX_VERTEX_UNIFORM_VECTORS = %d", maxUniformVectorsVs);
        eslog("GL_MAX_FRAGMENT_UNIFORM_VECTORS = %d", maxUniformVectorsFs);
    }
    eslog("-----------------------------");
}

/**
 * レンダラー名を取得する
 */
const std::string &GPUCapacity::getRenderer() {
    return renderer;
}

/**
 * ベンダー名を取得する
 */
const std::string &GPUCapacity::getVendor() {
    return vendor;
}

/**
 * 頂点属性の最大数を取得する。
 */
uint32_t GPUCapacity::getMaxVertexAttributes() {
    return maxVertexAttrbs;
}

/**
 * テクスチャユニットの最大数を取得する
 * この枚数を超えるテクスチャ処理は行えない。
 */
uint32_t GPUCapacity::getMaxTextureUnits() {
    return maxTextureUnits;
}

/**
 * テクスチャの一辺のサイズの最大値
 */
uint32_t GPUCapacity::getMaxTextureSize() {
    return maxTextureSize;
}

/**
 * 頂点シェーダのUniformベクトル最大数を取得する
 */
uint32_t GPUCapacity::getMaxUniformVectorsVs() {
    return maxUniformVectorsVs;
}

/**
 * フラグメントシェーダでのUniformベクトル最大数を取得する
 */
uint32_t GPUCapacity::getMaxUniformVectorsFs() {
    return maxUniformVectorsFs;
}

/**
 * GPU拡張機能をサポートするかを調べる
 */
bool GPUCapacity::isSupport(const GPUExtension_e extension) {
    return extension_flags.isEnable(extension);
}

/**
 * GPU系列のチェックを行う
 */
GPUFamily_e GPUCapacity::getGPUFamily() {
    return gpuFamily;
}

const std::string &GPUCapacity::getVersion() {
    return version;
}

const OpenGLVersion_e GPUCapacity::getGLVersion() {
    return glVersion;
}

const bool GPUCapacity::isOpenGLES() {
    return (glVersion % 10) == 0;
}
}
