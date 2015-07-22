#include "FontCharactor.h"

namespace es {

const Vector2i16 &FontCharactor::getBitmapSize() const {
    return bitmapSize;
}

const int16_t FontCharactor::getBitmapBearingY() const {
    return bitmapBearingY;
}

const wchar_t FontCharactor::getCode() const {
    return charactor;
}

const Vector2i16 &FontCharactor::getBitmapOffset() const {
    return bitmapOffset;
}

const Vector2i16 &FontCharactor::getAdvanceSize() const {
    return advance;
}

}