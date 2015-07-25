#include "NanoTimer.h"
#include "es/system/Thread.hpp"

namespace es {

void NanoTimer::sleepMs(const uint ms) {
    std::this_thread::sleep_for(
            std::chrono::milliseconds(ms)
    );
}

}
