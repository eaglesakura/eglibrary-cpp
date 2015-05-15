//
// Created by @eaglesakura on 15/05/15.
//

#include <es/util/StringUtil.h>
#include "VmdFileLoader.h"
#include "MmdBufferDataLoader.h"

namespace es {

namespace {
struct VmdRawHeader {
    uint8_t magic[30];
    
    uint8_t modelName[20];
};

}

VmdFileLoader::VmdFileLoader() {
    encoder.reset(new StringEncoder(CHARSET_UTF8, CHARSET_SJIS));
}

MVmdMotionController VmdFileLoader::loadVmd(const unsafe_array<uint8_t> buffer) {
    MVmdMotionController result(new VmdMotionController());
    
    MmdBufferDataLoader loader(buffer);
    
    {
        VmdRawHeader header = {0};
        loader.loadBuffer(&header, sizeof(VmdRawHeader));
        StringEncoder encoder("UTF-8", "SJIS");
//        std::string modelName = sjisToUTF8((char *) header.modelName, 30);
        std::string modelName = encoder.encode(header.modelName);
        
        eslog("magic(%s), model(%s)", header.magic, modelName.c_str());
        result->setModelName(modelName);
    }
    
    if (!loadBones(&loader, result)) {
        return MVmdMotionController();
    }
    
    return result;
}

bool VmdFileLoader::loadBones(MmdBufferDataLoader *loader, MVmdMotionController result) {
    
    const uint numFrames = loader->loadInt32();
    eslog("numFrames(%d)", numFrames);

    for (int i = 0; i < numFrames; ++i) {
        std::string boneName = loader->loadEncodedString(15, encoder.get());
        eslog("Frame[%d] target(%s)", i, boneName.c_str());

        MVmdBoneMotionController controller = result->findBoneController(boneName);
        assert(controller);

        VmdKeyFrame *key = controller->newKeyFrame(loader->loadInt32());
        loader->loadBuffer(&key->pos, sizeof(vec3));
        loader->loadBuffer(&key->rotate, sizeof(quat));
        loader->loadBuffer(key->bezier, 64);

        eslog("    Key[%d] T(%.2f, %.2f, %.2f) R(%.2f, %.2f, %.2f, %.2f)", key->frame, key->pos.x, key->pos.y, key->pos.z, key->rotate.x, key->rotate.y, key->rotate.z, key->rotate.w);
    }
    
    return true;
}
}