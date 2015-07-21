#include "FonteTextureAtlas.h"

namespace es {

FontTextureAtlas::FontTextureAtlas(const std::shared_ptr<FontFace> newFont) : font(newFont) {
    assert(font);
}

uint FontTextureAtlas::bake(const std::wstring text) {
    return 0;
}

const std::vector<std::shared_ptr<es::Texture> > &FontTextureAtlas::getTextures() const {
    return textures;
}

}
