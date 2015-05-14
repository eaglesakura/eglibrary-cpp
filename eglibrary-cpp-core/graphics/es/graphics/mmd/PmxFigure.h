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

    /**
     * ボーン一覧
     */
    std::vector<MPmxBone> bones;
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

    void addBone(MPmxBone bone) {
        bones.push_back(bone);
    }

    uint getBoneCount() const {
        return (uint) bones.size();
    }

    MPmxBone getBone(uint index) const {
        assert(index < bones.size());
        return bones[index];
    }
};

typedef std_shared_ptr<PmxFigure> MPmxFigure;

}


#endif //ASSEMBLE_PMXFIGURE_H
