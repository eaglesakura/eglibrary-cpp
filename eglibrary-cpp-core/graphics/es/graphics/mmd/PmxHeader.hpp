//
// Created by @eaglesakura on 15/05/08.
//

#ifndef ASSEMBLE_PMXHEADER_HPP
#define ASSEMBLE_PMXHEADER_HPP

#include    "es/eglibrary.hpp"

namespace es {

struct PmxHeader {
    /**
     * PmxVersion
     */
    float version;

    /**
     * TextEncodeType
     *
     * 0:UTF-16
     * 1:UTF-8
     */
    uint8_t encodeType : 1;

    /**
     * 追加UV数
     *
     * 0〜4
     */
    uint8_t addUVCount : 3;

    /**
     * 頂点Indexのサイズ(bytes)
     *
     * 1 : uint8_t
     * 2 : uint16_t
     * 4 : uint32_t
     */
    uint8_t vertexIndexSize : 3;

    /**
     * テクスチャインデックスのサイズ(bytes)
     *
     * 1 : uint8_t
     * 2 : uint16_t
     * 4 : uint32_t
     */
    uint8_t textureIndexSize : 3;

    /**
     * マテリアルインデックスのサイズ(bytes)
     *
     * 1 : uint8_t
     * 2 : uint16_t
     * 4 : uint32_t
     */
    uint8_t materialIndexSize : 3;

    /**
     * ボーンインデックスのサイズ(bytes)
     *
     * 1 : uint8_t
     * 2 : uint16_t
     * 4 : uint32_t
     */
    uint8_t boneIndexSize : 3;

    /**
     * モーフインデックスのサイズ(bytes)
     *
     * 1 : uint8_t
     * 2 : uint16_t
     * 4 : uint32_t
     */
    uint8_t morphIndexSize : 3;

    /**
     * 剛体インデックスのサイズ(bytes)
     *
     * 1 : uint8_t
     * 2 : uint16_t
     * 4 : uint32_t
     */
    uint8_t rigidIndexSize : 3;
};

/**
 * ヘッダに書き込まれたモデル情報
 */
class PmxModelInfo {
public:
    std::string name;
    std::string nameEng;

    std::string comment;
    std::string commentEng;
};

}

#endif //ASSEMBLE_PMXHEADER_HPP
