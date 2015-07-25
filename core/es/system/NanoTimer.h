#pragma once

#include    "es/eglibrary.hpp"
#include    <chrono>

namespace es {
/**
 * ナノ秒単位の測定を行う高分解能タイマー
 */
class NanoTimer {
public:
    NanoTimer() {
        start();
        end();
    }

    ~NanoTimer() = default;

    NanoTimer &start() {
        startTime = std::chrono::high_resolution_clock::now();
        return *this;
    }

    NanoTimer &end() {
        endTime = std::chrono::high_resolution_clock::now();
        return *this;
    }

    /**
     * 秒単位で取得する
     */
    double second() {
        return ((double) std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count()) / 1000.0 / 1000.0 / 1000.0;
    }

    /**
     * ミリ秒単位で取得する
     */
    double milliSecond() {
        return ((double) std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count()) / 1000.0 / 1000.0;
    }

    /**
     * マイクロ秒単位で取得する
     */
    double microSecond() {
        return ((double) std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count()) / 1000.0;
    }

    static void sleepMs(const uint ms);

private:
    std::chrono::high_resolution_clock::time_point startTime;
    std::chrono::high_resolution_clock::time_point endTime;
};
}
