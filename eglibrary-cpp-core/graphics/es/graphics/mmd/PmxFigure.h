//
// Created by @eaglesakura on 15/05/08.
//

#ifndef ASSEMBLE_PMXFIGURE_H
#define ASSEMBLE_PMXFIGURE_H


#include    <vector>
#include    "es/eglibrary.hpp"
#include    "PmxMesh.h"
#include    "PmxMaterial.h"
#include    "PmxBone.h"

namespace es {


class PmxFigure : public Object {
    /**
     * レンダリング用メッシュ
     */
    MPmxMesh mesh;

    std::vector<MPmxMaterial> materials;

    MPmxBoneData boneData;

public:

    PmxFigure() { }


    virtual ~PmxFigure() { }


    MPmxMesh getMesh() const {
        return mesh;
    }

    void setMesh(const MPmxMesh &mesh) {
        PmxFigure::mesh = mesh;
    }

    void addMaterial(MPmxMaterial material) {
        materials.push_back(material);
    }

    uint getMaterialCount() const {
        return (uint) materials.size();
    }

    MPmxMaterial getMaterial(uint index) const {
        assert(index < materials.size());
        return materials[index];
    }

    const MPmxBoneData &getBoneData() const {
        return boneData;
    }

    void setBoneData(const MPmxBoneData &boneData) {
        PmxFigure::boneData = boneData;
    }
};

typedef std_shared_ptr<PmxFigure> MPmxFigure;

}


#endif //ASSEMBLE_PMXFIGURE_H
