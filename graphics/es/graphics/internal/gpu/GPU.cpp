#include    "es/graphics/GPU.h"
#include    "es/memory/BitFlags.hpp"
#include    <vector>
#include    "es/util/StringUtil.h"
#include    "es/graphics/gl/context/GLContextUtil.hpp"

#include "es/graphics/internal/gpu/GPU_gl.hpp"

namespace es {

/**
 * レンダラー名を取得する
 */
const std::string &GPU::getRenderer() {
    return self()->renderer;
}

/**
 * ベンダー名を取得する
 */
const std::string &GPU::getVendor() {
    return self()->vendor;
}

/**
 * 頂点属性の最大数を取得する。
 */
uint32_t GPU::getMaxVertexAttributes() {
    return self()->gl.maxVertexAttrbs;
}

/**
 * テクスチャユニットの最大数を取得する
 * この枚数を超えるテクスチャ処理は行えない。
 */
uint32_t GPU::getMaxTextureUnits() {
    return self()->maxTextureUnits;
}

/**
 * テクスチャの一辺のサイズの最大値
 */
uint32_t GPU::getMaxTextureSize() {
    return self()->maxTextureSize;
}

/**
 * 頂点シェーダのUniformベクトル最大数を取得する
 */
uint32_t GPU::getMaxUniformVectorsVs() {
    return self()->gl.maxUniformVectorsVs;
}

/**
 * フラグメントシェーダでのUniformベクトル最大数を取得する
 */
uint32_t GPU::getMaxUniformVectorsFs() {
    return self()->gl.maxUniformVectorsFs;
}

/**
 * GPU拡張機能をサポートするかを調べる
 */
bool GPU::isSupport(const GPUExtension_e extension) {
    return self()->extension_flags.isEnable(extension);
}

/**
 * GPU系列のチェックを行う
 */
GPUFamily_e GPU::getGPUFamily() {
    return self()->gpuFamily;
}

const std::string &GPU::getVersion() {
    return self()->version;
}

const OpenGLVersion_e GPU::getGLVersion() {
    return self()->gl.glVersion;
}

const bool GPU::isOpenGLES() {
    if (getEngine() != GraphicsEngine_OpenGL) {
        // GLでなければESはあり得ない
        return false;
    }
    return (self()->gl.glVersion % 10) == 0;
}

bool GPU::isFamily(const GPUFamily_e family) {
    return getGPUFamily() == family;
}

const std::string &GPU::getGlslVersion() {
    return self()->shaderVersion;
}

sp<GPU::Impl> GPU::self() {
    static sp<GPU::Impl> impl;
    if (!impl) {
        impl.reset(new Impl());

        impl->glInitialize();
    }
    return impl;
}

const GraphicsEngine_e GPU::getEngine() {
#if defined(ES_GRAPHICS_OPENGL)
    return GraphicsEngine_OpenGL;
#else
    return GraphicsEngine_Unknown;
#endif
}

}
