#pragma once

#include    "es/Graphics.hpp"

// math
#include    "es/graphics/math/GlmHelper.hpp"

// base
#include    "es/graphics/gl/surface/VirtualDisplay.hpp"
#include    "es/graphics/gl/surface/IRenderingSurface.hpp"
#include    "es/graphics/gl/GLObject.h"
#include    "es/graphics/Color.hpp"
#include    "es/graphics/PixelFormat.hpp"
#include    "es/graphics/gl/gpu/GPUCapacity.h"
#include    "es/graphics/gl/context/DeviceContext.h"

// resources
#include    "es/graphics/gl/resource/Texture.h"
#include    "es/graphics/gl/surface/RenderBufferObject.hpp"
#include    "es/graphics/gl/surface/FrameBufferObject.hpp"
#include    "es/graphics/gl/resource/ShaderProgram.h"
#include    "es/graphics/gl/resource/VertexBufferObject.hpp"
#include    "es/graphics/gl/resource/IndexBufferObject.hpp"

// shader
#include    "es/graphics/gl/shader/UniformBase.hpp"
#include    "es/graphics/gl/shader/VectorUniform.hpp"
#include    "es/graphics/gl/shader/ColorUniform.hpp"
#include    "es/graphics/gl/shader/TextureUniform.hpp"
#include    "es/graphics/gl/shader/VertexAttribute.hpp"
