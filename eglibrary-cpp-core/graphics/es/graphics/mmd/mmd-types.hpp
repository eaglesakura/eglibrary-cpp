//
// Created by @eaglesakura on 15/05/28.
//

#ifndef ASSEMBLE_MMD_TYPES_HPP
#define ASSEMBLE_MMD_TYPES_HPP

#include    <string>
#include    <stdint.h>
#include    <vector>
#include    <map>

/**
 * ライブラリはglmに依存する
 */
#define GLM_FORCE_RADIANS
#define GLM_SWIZZLE
#define GLM_FORCE_INLNIE

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>


namespace mmd {

typedef ::glm::vec2 vec2;
typedef ::glm::vec3 vec3;
typedef ::glm::vec4 vec4;
typedef ::glm::mat3 mat3;
typedef ::glm::mat4 mat4;
typedef ::glm::quat quat;

/**
 * アクセスに注意が必要なポインタを扱う
 */
template<typename T>
struct unsafe_ptr {
    /**
     * ポインタ本体
     */
    T *ptr;

    /**
     * 配列の長さ
     */
    uint32_t length;
};

/**
 * メモリサイズの検証
 */
static_assert(sizeof(float) == 4, "float != 4byte");
static_assert(sizeof(double) == 8, "double != 4byte");
static_assert(sizeof(mat4) == (sizeof(float) * 16), "sizeof(mat4) != float[4][4]");
static_assert(sizeof(vec2) == (sizeof(float) * 2), "sizeof(vec2) != float[2]");
static_assert(sizeof(vec3) == (sizeof(float) * 3), "sizeof(vec3) != float[3]");
static_assert(sizeof(vec4) == (sizeof(float) * 4), "sizeof(vec4) != float[4]");
static_assert(sizeof(quat) == (sizeof(float) * 4), "sizeof(vec4) != float[4]");

}

#endif //ASSEMBLE_MMD_TYPES_HPP
