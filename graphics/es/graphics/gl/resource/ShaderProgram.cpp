#include    "es/OpenGL.hpp"

namespace es {

ShaderProgram::ShaderProgram(const GLuint program) {
    this->program = program;
    assert(program);

    // uniform / attribute
    {
        GLint temp = 0;
        glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &temp);
        this->maxAttributes = temp;
        glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &temp);
        this->maxUniforms = temp;
    }
}

ShaderProgram::~ShaderProgram() {
    dispose();
}

/**
 * 管理しているリソースの解放を行う
 */
void ShaderProgram::dispose() {
    if (!program) {
        return;
    }

    glDeleteProgram(program);
    program = 0;
    maxAttributes = maxUniforms = 0;
}

/**
 * プログラム属性変数のロケーションを取得する
 */
GLint ShaderProgram::getAttribLocation(const char *name) const {
    if (!program) {
        return -1;
    }
    return glGetAttribLocation(program, name);
}

/**
 * uniform変数のロケーションを取得する
 */
GLint ShaderProgram::getUniformLocation(const char *name) const {
    if (!program) {
        return -1;
    }
    return glGetUniformLocation(program, name);
}

/**
 * GLと関連付ける
 */
void ShaderProgram::bind() {
    glUseProgram(program);
}

namespace {

/**
 * シェーダーの読み込みを行う。
 */
static GLint compileShader(const char* shader_source, const GLenum GL_XXXX_SHADER) {
    const GLint shader = glCreateShader(GL_XXXX_SHADER);
    assert_gl();

    glShaderSource(shader, 1, &shader_source, NULL);
    glCompileShader(shader);

    // コンパイルエラーをチェックする
    {
        GLint compileSuccess = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compileSuccess);
        if (compileSuccess == GL_FALSE) {
            // エラーが発生した
            GLint infoLen = 0;
            // エラーメッセージを取得
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen > 1) {
                GLchar *msg = (GLchar*) calloc(infoLen, sizeof(GLchar));
                glGetShaderInfoLog(shader, infoLen, NULL, msg);

                eslog("%s", msg);
                free((void*) msg);
            } else {
                eslog("comple error not info...");
            }
        }

        // コンパイル失敗していたら0を返却する
        if (compileSuccess != GL_TRUE) {
            glDeleteShader(shader);
            return 0;
        }
    }

    return shader;
}

/**
 * 頂点・フラグメントシェーダーをリンクし、ShaderProgramを作成する
 */
static GLuint buildProgram(const char* vertex_shader_source, const char* fragment_shader_source) {
    const GLuint vertex_shader = compileShader(vertex_shader_source, GL_VERTEX_SHADER);
    if (!vertex_shader) {
        eslog("vertex shader compile error");
        return 0;
    }

    const GLuint fragment_shader = compileShader(fragment_shader_source, GL_FRAGMENT_SHADER);
    if (!fragment_shader) {
        eslog("fragment shader compile error");
        return 0;
    }

    const GLuint program = glCreateProgram();
    assert_gl();
    assert(program != 0);

    glAttachShader(program, vertex_shader); // バーテックスシェーダーとプログラムを関連付ける
    glAttachShader(program, fragment_shader); // フラグメントシェーダーとプログラムを関連付ける
    assert_gl();

    // コンパイルを行う
    glLinkProgram(program);

    // リンクエラーをチェックする
    {
        GLint linkSuccess = 0;
        glGetProgramiv(program, GL_LINK_STATUS, &linkSuccess);
        if (linkSuccess == GL_FALSE) {
            // エラーが発生した
            GLint infoLen = 0;
            // エラーメッセージを取得
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen > 1) {
                GLchar *msg = (GLchar*) calloc(infoLen, sizeof(GLchar));
                glGetProgramInfoLog(program, infoLen, NULL, msg);
                eslog("%s", msg);
                free((void*) msg);
            }
        }

        // コンパイル失敗していたら0を返却する
        if (linkSuccess != GL_TRUE) {
            eslog("link error ...");
            if (vertex_shader) {
                glDeleteShader(vertex_shader);
            }
            if (fragment_shader) {
                glDeleteShader(fragment_shader);
            }
            glDeleteProgram(program);
            return 0;
        }
    }

    // リンク済みのため、個々のシェーダーオブジェクトの解放フラグを立てる
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    // リンク済みのプログラムを返す
    return program;
}

}
/**
 * ビルドを行う
 */
::std::shared_ptr<ShaderProgram> ShaderProgram::build(const char* vertex_shader, const char* frament_shader, MDeviceContext state) {
    GLuint program = buildProgram(vertex_shader, frament_shader);
    if (!program) {
        eslog("error vert shader\n%s", vertex_shader);
        eslog("error frag shader\n%s", frament_shader);

        return std::shared_ptr<ShaderProgram>();
    }

    std::shared_ptr<ShaderProgram> result(new ShaderProgram(program));
    return result;
}
}
