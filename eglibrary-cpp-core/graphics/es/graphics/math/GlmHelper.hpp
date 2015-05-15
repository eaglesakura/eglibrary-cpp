//
// GLMの独自サポート系
//

#ifndef ASSEMBLE_GLMHELPER_HPP
#define ASSEMBLE_GLMHELPER_HPP


#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include    "es/math/Vector2.hpp"
#include    "es/math/Vector3.hpp"
#include    "es/math/Vector4.hpp"
#include    "es/math/Matrix.hpp"

namespace   es {

typedef ::glm::vec2 vec2;
typedef ::glm::vec3 vec3;
typedef ::glm::vec4 vec4;
typedef ::glm::mat4 mat4;


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
