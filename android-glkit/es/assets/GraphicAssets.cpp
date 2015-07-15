#include    "GraphicAssets.h"
#include    "es_glkit_GraphicAssets.h"
#include    "es_glkit_DecodedImage.h"

using namespace ::jc::lang;
using namespace ::jc;

namespace es {

/**
 *
 */
GraphicAssets::GraphicAssets(::jc::lang::object_wrapper jApplicationContext) {
    this->jApplicationContext = jApplicationContext.globalRef();
    eslog("jApplicationContext(%x)", this->jApplicationContext.getJobject());
    this->jAssetManager = es::glkit::GraphicAssets::getAssets(this->jApplicationContext);
    eslog("jAssetManager(%x)", this->jAssetManager.getJobject());
    this->jAssetManager.globalRef();

    this->assetManager.reset(new AndroidAssetManager(jAssetManager));
}

GraphicAssets::~GraphicAssets() {

}

/**
 * assets配下から読み出す
 */
MTexture GraphicAssets::loadTextureFromAssets(const std::string &path, const TextureAssetType_e type, const TextureLoadOption *option) {
    TextureLoadOption _opt;
    TextureLoadOption *opt = (TextureLoadOption *) option;
    if (!opt) {
        opt = &_opt;
    }

    eslog("asset load(%s)", path.c_str());

    object_wrapper bitmap = es::glkit::GraphicAssets::loadImageFromAssets(jApplicationContext, path);
    if (!bitmap.hasObject()) {
        eslog("decode failed");
        return MTexture();
    } else if (opt->load_cancel) {
        eslog("load cancel");
        return MTexture();
    }

    object_wrapper decoded = es::glkit::GraphicAssets::decodeImage(bitmap, JNI_TRUE);
    assert(decoded.hasObject());

    if (opt->load_cancel) {
        eslog("load cancel");
        return MTexture();
    }

    // デコードオブジェクトから情報を取り出す
    es::glkit::DecodedImage image(decoded);

    GLint imageWidth = image.getWidth();
    GLint imageHeight = image.getHeight();

    eslog("decoded image size(%d x %d)", image.getWidth(), image.getHeight());

    object_wrapper pixels = image.getPixels();
    JNIEnv *env = jc::jni::getThreadJniEnv();
    void *pPixelBuffer = env->GetDirectBufferAddress(pixels.getJobject());

    MDeviceContext context = DeviceContext::current();
    MShaderState state = DeviceContext::current()->getShaderState();
    MTexture result(new Texture());
    assert_gl();

    result->bind(context);
    assert_gl();

    if (Pixel::getPixelBytes(opt->format) == 4) {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    } else {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    }
    assert_gl();

    if (opt->format == PixelFormat_NativeRGBA) {
        // コンバートが不要である
        glTexImage2D(GL_TEXTURE_2D, 0, Pixel::toGLPixelFormat(PixelFormat_NativeRGBA), imageWidth, imageHeight, 0, Pixel::toGLPixelFormat(PixelFormat_NativeRGBA), GL_UNSIGNED_BYTE, pPixelBuffer);
    } else {
        eslog("convert pixelformat(%d -> %d)", PixelFormat_NativeRGBA, opt->format);
        // コンバートを行う
        ByteBuffer buffer = Pixel::createPixelBuffer(opt->format, imageWidth * imageHeight);
        Pixel::copyBGRA8888Pixels((uint8_t*) pPixelBuffer, opt->format, buffer.get(), imageWidth * imageHeight);

        // 転送を行う
        glTexImage2D(GL_TEXTURE_2D, 0, Pixel::toGLPixelFormat(opt->format), imageWidth, imageHeight, 0, Pixel::toGLPixelFormat(opt->format), Pixel::toGLPixelDataType(opt->format), buffer.get());
    }
    assert_gl();

    result->setWrapMode(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
    result->setFilter(GL_NEAREST, GL_NEAREST);

    // mipmap
    if (opt->gen_mipmap && result->isPowerOfTwoTexture()) {
        eslog("gen mipmap");
        result->genMipmaps();
        assert_gl();
    }
    result->unbind(context);
    assert_gl();

    // サイズ補正
    result->size.img_width = result->size.tex_width = imageWidth;
    result->size.img_height = result->size.tex_height = imageHeight;

    // alloc成功通知
    result->onAllocated();

    if (opt->load_cancel) {
        eslog("load cancel");
        return MTexture();
    }

    return result;
}

/**
 * assets配下からシェーダーを読み出す
 */
shared_ptr<ShaderProgram> GraphicAssets::loadShaderFromAssets(const std::string &vertShaderFile, const std::string &fragShaderFile) {
    std::string vertShader = assetManager->loadText(vertShaderFile.c_str());
    std::string fragShader = assetManager->loadText(fragShaderFile.c_str());

    if (vertShader.empty()) {
        eslog("vert shader empty(%s)", vertShaderFile.c_str());
    }

    if (fragShader.empty()) {
        eslog("frag shader empty(%s)", fragShader.c_str());
    }

    eslog("build vsh(%s) fsh(%s)", vertShaderFile.c_str(), fragShaderFile.c_str());
    return ShaderProgram::build(vertShader.c_str(), fragShader.c_str());
}

/**
 * SpriteManagerを生成する
 */
MSpriteManager GraphicAssets::createSpriteManager(const std::string &vertShaderFile, const std::string &fragShaderFile) {
    shared_ptr<ShaderProgram> spriteShader = loadShaderFromAssets(vertShaderFile, fragShaderFile);
    if (!spriteShader) {
        return MSpriteManager();
    }

    assert(spriteShader);
    MSpriteManager spriteManager(new SpriteManager());
    spriteManager->bind();
    spriteManager->setShader(spriteShader);
    spriteManager->unbind();
    assert_gl();

    return spriteManager;
}

/**
 * カメラを取得する
 */
MCameraDeviceManager GraphicAssets::connectCameraDevice(const CameraDeviceMode_e deviceType) {

    auto jCameraTextureRenderer = glkit::GraphicAssets::connectCamera(jApplicationContext, deviceType);
    if (!jCameraTextureRenderer.hasObject()) {
        eslog("camera connect failed");
        return MCameraDeviceManager();
    }

    ::std::shared_ptr<glkit::CameraTextureRenderer> camera(new glkit::CameraTextureRenderer(jCameraTextureRenderer.globalRef()));
    MCameraDeviceManager result(new CameraDeviceManager(camera));

    return result;
}

}
