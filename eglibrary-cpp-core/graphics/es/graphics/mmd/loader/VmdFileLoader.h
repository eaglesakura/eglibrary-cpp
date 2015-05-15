//
// Created by @eaglesakura on 15/05/15.
//

#ifndef ASSEMBLE_VMDFILELOADER_H
#define ASSEMBLE_VMDFILELOADER_H

#include    "es/graphics/mmd/mmd.hpp"
#include "MmdBufferDataLoader.h"

namespace   es {
class VmdFileLoader {
    std_shared_ptr<StringEncoder> encoder;

    virtual bool loadBones(MmdBufferDataLoader *loader, MVmdMotionController result);

public:
    VmdFileLoader();

    virtual ~VmdFileLoader() { }

    virtual MVmdMotionController loadVmd(const unsafe_array<uint8_t> buffer);
};

}


#endif //ASSEMBLE_VMDFILELOADER_H
