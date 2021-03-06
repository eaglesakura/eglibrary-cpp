/*
 * BitFlags.hpp
 *
 *  Created on: 2014/09/08
 *      Author: eaglesakura
 */

#ifndef es_memory_BITFLAGS_HPP_
#define es_memory_BITFLAGS_HPP_

#include    "es/eglibrary.hpp"

template<int32_t MAX_FLAG_NUM = 32>
class BitFlags {
    /**
     * フラグ
     */
    int32_t flags[(MAX_FLAG_NUM + 31) / 32];
public:
    BitFlags() {
        reset();
    }

    ~BitFlags() {
    }

    void reset() {
        for (int32_t i = 0; i < (sizeof(flags) / sizeof(int32_t)); ++i) {
            flags[i] = 0x00000000;
        }
    }

    /**
     * フラグを有効にする
     */
    void enable(const int32_t index) {
        assert((index / 32) < ((sizeof(flags) / sizeof(int32_t))));

        flags[index / 32] |= (0x1 << (index % 32));
    }

    /**
     * フラグを無効化する
     */
    void disable(const int32_t index) {
        assert((index / 32) < ((sizeof(flags) / sizeof(int32_t))));

        flags[index / 32] &= ~(0x1 << (index % 32));
    }

    /**
     * フラグを立てる
     */
    void set(const int32_t index, const bool set) {
        if (set) {
            enable(index);
        } else {
            disable(index);
        }
    }

    /**
     * 特定インデックスがtrueであることを確認する
     */
    bool isEnable(const int32_t index) const {
        assert((index / 32) < ((sizeof(flags) / sizeof(int32_t))));

        return (flags[index / 32] & (0x1 << (index % 32))) != 0;
    }

    /**
     * 特定インデックスがfalseであることを確認する
     */
    bool isDisable(const int32_t index) const {
        return !isEnable(index);
    }

    /**
     * ビット演算でまとめたフラグがONになっていることを確認する
     * flags[0]に対してのみ対応可能なため、注意すること
     */
    bool isAllEnableFlags(const int32_t bits) const {
        return has_flag_all(flags[0], bits);
    }

    /**
     * 全チェック
     */
    bool isAllEnable(const int32_t indices_num, const int32_t* indices) const {
        for (int32_t i = 0; i < indices_num; ++i) {
            if (isDisable(indices[i])) {
                // 一つでもdisableがあったら廃棄
                return false;
            }
        }
        // 全てenable
        return true;
    }

    /**
     * 全チェック
     */
    bool isAllDisable(const int32_t indices_num, const int32_t* indices) const {
        for (int32_t i = 0; i < indices_num; ++i) {
            if (isEnable(indices[i])) {
                // 一つでもenableがあったら廃棄
                return false;
            }
        }
        // 全てenable
        return true;
    }
};

#endif /* BITFLAGS_HPP_ */
