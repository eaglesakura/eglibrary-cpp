LOCAL_PATH := $(call my-dir)
############  Module : eglibrary-glkit  ############
include $(CLEAR_VARS)
###### Includes
LOCAL_MODULE := eglibrary-glkit

###### Includes
LOCAL_C_INCLUDES += src
LOCAL_C_INCLUDES += jcgen

###### Sources
LOCAL_SRC_FILES += src/impl/es_glkit_GLKitUtil.cpp
LOCAL_SRC_FILES += jcgen/es_glkit_GLKitUtil_impl.cpp

###### Flags
LOCAL_CFLAGS += -Wno-psabi
LOCAL_CPPFLAGS += -fexceptions
LOCAL_CPPFLAGS += -pthread
LOCAL_CPPFLAGS += -frtti
LOCAL_CPPFLAGS += -std=c++0x

###### Libs
LOCAL_LDLIBS += -lEGL
LOCAL_LDLIBS += -lGLESv1_CM
LOCAL_LDLIBS += -lGLESv3
LOCAL_LDLIBS += -landroid
LOCAL_LDLIBS += -llog
LOCAL_LDLIBS += -ljnigraphics

###### Build
include $(BUILD_SHARED_LIBRARY)

