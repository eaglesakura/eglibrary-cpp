#include "MmdBufferDataLoader.h"
#include "es_glkit_JointMessage.h"

namespace es {

/**
 * 文字数を指定して識別子を読み込む
 */
std::string MmdBufferDataLoader::loadTextBuffer() {
    uint32_t len;
    loadBuffer(&len, 4);
    if (len == 0) {
        return std::string();
    }

    // バッファを変換する

    // 文字列を変換する
    if (encodeType == UTF16) {
        JNIEnv *env = jc::jni::getThreadJniEnv();
        const jbyteArray array = (jbyteArray) env->NewByteArray(len);
        // データをコピーする
        {
            env->SetByteArrayRegion(array, 0, len, (jbyte *) this->buffer.ptr);
            // バッファを読み進める
            this->buffer += len;
        }
        // データを変換する
        const jc::lang::object_wrapper cnv = glkit::JointMessage::utf16toString(array);
        int convertedLength = env->GetArrayLength((jbyteArray) cnv.getJobject());

        safe_array<uint8_t> buffer(convertedLength + 1);
        buffer.zeromemory();

        // データを書き戻す
        env->GetByteArrayRegion((jbyteArray) cnv.getJobject(), 0, convertedLength, (jbyte *) buffer.ptr);

        // 配列を解放する
        env->DeleteLocalRef(array);

        return std::string((char *) buffer.ptr);
    } else {
        safe_array<uint8_t> buffer(len + 1);
        loadBuffer(buffer.ptr, (uint) len);
        buffer.ptr[len] = 0;

        return std::string((char *) buffer.ptr);
    }

}
}