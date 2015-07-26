#include "FontFace.h"

namespace es {

const Vector2i16 &FontFace::getSize() {
    return size;
}

void FontFace::setThreadSafe(bool set) {
    threadSafe = set;
}

}