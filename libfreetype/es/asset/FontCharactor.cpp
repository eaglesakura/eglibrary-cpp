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

Vector2i16 FontCharactor::getLayoutSize(bool withLeft, bool withRight) const {
    Vector2i16 size = getFontSize();
    uint advanceHalf = (advanceWidth - size.x / 2);
    if (advanceHalf > 0) {
        if (withLeft) {
            size.x += advanceHalf;
        }
        if (withRight) {
            size.y += advanceHalf;
        }
    }
    return size;
}

const Vector2i16 &FontCharactor::getFontSize() const {
    return fontSize;
}

const Vector2i16 &FontCharactor::getBitmapOffset() const {
    return bitmapOffset;
}

const Vector2i16 &FontCharactor::getAdvanceSize() const {
    return advance;
}
}