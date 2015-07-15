#if defined(__ANDROID__) && defined(ES_BUILD_GTEST_MODE)

#include "es/eglibrary.hpp"
#include "JointConnector.hpp"

/**
 * ディレクトリ単位でビルドされるため、必要なファイルのみをincludeして対応する
 */
#include "src/gtest-all.cc"

extern "C" {

/**
 * gtest形式のユニットテストを実行する
 */
JNIEXPORT jint JNICALL Java_com_eaglesakura_android_testing_NdkUnitTestSupport_ndkTestMain(JNIEnv *env, jstring modulePath, jstring functionName, jstring exportXmlPath) {
    return 0;
}


}

#endif