//
// Created by @eaglesakura on 15/05/08.
//

#ifndef ASSEMBLE_PMXFIGURE_H
#define ASSEMBLE_PMXFIGURE_H


#include    "es/eglibrary.hpp"
#include "PmxMesh.h"

namespace es {


class PmxFigure : public Object {
    /**
     * レンダリング用メッシュ
     */
    MPmxMesh mesh;
public:

    PmxFigure() { }


    virtual ~PmxFigure() { }


    MPmxMesh getMesh() const {
        return mesh;
    }

    void setMesh(const MPmxMesh &mesh) {
        PmxFigure::mesh = mesh;
    }
};

typedef std_shared_ptr<PmxFigure> MPmxFigure;

}


#endif //ASSEMBLE_PMXFIGURE_H
