#ifdef __ANDROID__

#pragma once

#include "es/eglibrary.hpp"
#include "JointConnector.hpp"

namespace es {

namespace internal {

class AndroidContextUtil {
public:
    /**
     * 初期化を行う
     *
     * @param jContext ApplicationContextを渡す。AppContextは実質的にシングルトンなので、プロセスで参照していても問題ない。
     */
    static void initialize(JNIEnv *env, jobject jContext);

    /**
     * AssetManagerを取得する
     */
    static ::jc::lang::object_wrapper getAssets();
};

}


}

#endif