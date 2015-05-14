//
// Created by @eaglesakura on 15/05/13.
//

#include "PmxBone.h"

namespace   es {

void PmxBone::allocIkLinkList(uint ikNum) {
    this->ikLinkList.reserve(ikNum);
}
}