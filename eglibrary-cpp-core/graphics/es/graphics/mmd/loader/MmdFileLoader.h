//
// Created by @eaglesakura on 15/05/08.
//

#ifndef ASSEMBLE_MMDFILELOADER_H
#define ASSEMBLE_MMDFILELOADER_H

#include "es/graphics/mmd/mmd.hpp"
#include "MmdBufferDataLoader.h"

namespace es {

/**
 * *.pmxファイルから読み込んだ情報
 */
class PmxFile {
public:
    /**
     * ヘッダ情報
     */
    std_shared_ptr<PmxHeader> header;

    /**
     * インフォメーション情報
     */
    std_shared_ptr<PmxModelInfo> info;

    /**
     * 読み込んだフィギュア本体
     */
    MPmxFigure figure;


    PmxFile() {
        header.reset(new PmxHeader());
        info.reset(new PmxModelInfo());
    }


    virtual ~PmxFile() { }
};

typedef std_shared_ptr<PmxFile> MPmxFile;

/**
 * MMD関連ファイルのローディングを行う
 */
class MmdFileLoader : public Object {

protected:
    /**
     * ヘッダを読み込む
     */
    virtual bool loadPmxHeader(MmdBufferDataLoader *loader, MPmxFile result);

    /**
     * 頂点情報を読み込む
     */
    virtual bool loadPmxVertices(MmdBufferDataLoader *loader, MPmxFile result);

    /**
     * インデックス情報を取得する
     */
    virtual bool loadPmxFaces(MmdBufferDataLoader *loader, MPmxFile result);

    /**
     * マテリアル情報を取得する
     */
    virtual bool loadPmxMaterials(MmdBufferDataLoader *loader, MPmxFile result);

public:
    MmdFileLoader();

    virtual ~MmdFileLoader();

    /**
     * PMXバッファからフィギュアを生成する
     */
    virtual MPmxFile loadPmx(const unsafe_array<uint8_t> buffer);
};

}


#endif //ASSEMBLE_MMDFILELOADER_H
