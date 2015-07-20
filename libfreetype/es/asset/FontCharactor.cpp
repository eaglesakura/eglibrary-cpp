#include "FontCharactor.h"

namespace es {

int FontCharactor::getAcent() const {
    return acent;
}

int FontCharactor::getDescent() const {
    return descent;
}

const Vector2i16 &FontCharactor::getBitmapSize() const {
    return bitmapSize;
}
}