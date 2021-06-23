
#ifndef GLSL
#define GLSL(A) "#version 300 es \n" #A
#endif

#include <GLES3/gl3.h>
#include <GLES2/gl2ext.h>

#ifndef _ANDROID_LOG_H_
#define _ANDROID_LOG_H_

#include <android/log.h>

#define TAG "System.out"

#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,    TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,     TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,     TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,    TAG, __VA_ARGS__)

#endif //SIMPLEPLAYER_LOG_H