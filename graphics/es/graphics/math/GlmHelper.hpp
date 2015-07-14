#pragma once

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace   es {

typedef ::glm::vec2 vec2;
typedef ::glm::vec3 vec3;
typedef ::glm::vec4 vec4;
typedef ::glm::mat3 mat3;
typedef ::glm::mat4 mat4;
typedef ::glm::quat quat;

/**
 * メモリサイズの検証
 */
static_assert(sizeof(mat4) == (sizeof(float) * 16), "sizeof(mat4) != float[4][4]");
static_assert(sizeof(vec2) == (sizeof(float) * 2), "sizeof(vec2) != float[2]");
static_assert(sizeof(vec3) == (sizeof(float) * 3), "sizeof(vec3) != float[3]");
static_assert(sizeof(vec4) == (sizeof(float) * 4), "sizeof(vec4) != float[4]");
static_assert(sizeof(quat) == (sizeof(float) * 4), "sizeof(vec4) != float[4]");

}
