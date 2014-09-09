#ifndef es_graphics_gl_QUAD_H_
#define es_graphics_gl_QUAD_H_

#include    "es/OpenGL.hpp"

namespace es {

/**
 * 四角形構築用の頂点構造体
 */
struct QuadVertex {
    float x;
    float y;

    float u;
    float v;
};

/**
 * 位置属性
 */
typedef VertexAttribute<QuadVertex, 2, GL_FLOAT, GL_FALSE, 0> QuadPositionAttribute;

/**
 * UV属性
 */
typedef VertexAttribute<QuadVertex, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2f)> QuadCoordAttribute;

/**
 * Quad Resource
 */
class Quad: public GLObject {

};

}

#endif /* QUAD_H_ */
