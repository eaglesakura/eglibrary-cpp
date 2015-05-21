//
// GLMの独自サポート系
//

#ifndef ASSEMBLE_GLMHELPER_HPP
#define ASSEMBLE_GLMHELPER_HPP


#define GLM_FORCE_RADIANS
#define GLM_SWIZZLE
#define GLM_FORCE_INLNIE

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include    "es/math/Vector2.hpp"
#include    "es/math/Vector3.hpp"
#include    "es/math/Vector4.hpp"
#include    "es/math/Matrix.hpp"

namespace   es {

typedef ::glm::vec2 vec2;
typedef ::glm::vec3 vec3;
typedef ::glm::vec4 vec4;
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


/**
 * pos -> endに向かうベクトルを計算する
 */
template<typename glmVecT>
inline glmVecT axis(const glmVecT &pos, const glmVecT &end) {
    return glm::normalize(end - pos);
}

/**
 * pos -> endに向かうベクトルを計算する。
 *
 * 0ベクトルの場合はdefの値をnormalizeして返却する
 */
template<typename glmVecT>
inline glmVecT axisSafe(const glmVecT &pos, const glmVecT &end, const glmVecT &defValue) {
    if (pos == end) {
        return glm::normalize(defValue);
    } else {
        return glm::normalize(end - pos);
    }
}


}

#endif //ASSEMBLE_GLMHELPER_HPP
