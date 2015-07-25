#pragma once

#include "es/graphics/GPU.h"
#include    "es/memory/BitFlags.hpp"

namespace es {

class GPU::Impl {
public:
    string renderer;
    string vendor;
    string version;
    string shaderVersion;
    /**
     * テクスチャユニットの最大数
     */
    uint32_t maxTextureUnits = 0;
    /**
     * テクスチャの一辺の最大サイズ
     */
    uint32_t maxTextureSize = 0;
    /**
     * 拡張設定
     */
    BitFlags<GPUExtension_Num> extension_flags;

    /**
     * GPUファミリー名
     */
    GPUFamily_e gpuFamily = GPUFamily_Unknown;

    struct {
        /**
         * 頂点属性の最大数
         */
        uint32_t maxVertexAttrbs = 0;

        /**
         * 頂点シェーダでの最大Uniformベクトル数
         */
        uint32_t maxUniformVectorsVs = 0;
        /**
         * フラグメントシェーダでの最大Uniformベクトル数
         */
        uint32_t maxUniformVectorsFs = 0;

        OpenGLVersion_e glVersion = OpenGLVersion_Unknown;
    } gl;

#if defined(ES_GRAPHICS_OPENGL)

    void glInitialize();

#endif
};

}