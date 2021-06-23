//
// Created by panyi on 2021/6/23.
//

#include <jni.h>
#include <string>
#include "glsl.h"
#include "shader.h"

class App{
public:
    void init();
    void resize(int w , int h);
    void render(long deltatime);
    void destory();

private:
    Shader shader;

    float vertexData[3 * 6] = {
            -1.0f , 1.0f ,0.0f ,
            -1.0f , -1.0f , 0.0f ,
            1.0f , 1.0f,  0.0f ,
            1.0f , 1.0f ,0.0f ,
            -1.0f , -1.0f , 0.0f ,
            1.0f , -1.0f,  0.0f ,
    };
};

static App *app = nullptr;

extern "C"
JNIEXPORT jstring JNICALL
Java_com_xinlan_shaderlab_NativeBridge_sayHello(JNIEnv *env, jobject thiz , jstring input) {
    std::string hello = "Hello from 耀西";
    return env->NewStringUTF(hello.c_str());
}

extern "C"
JNIEXPORT void JNICALL
Java_com_xinlan_shaderlab_NativeBridge_init(JNIEnv *env, jobject thiz) {
    app = new App();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_xinlan_shaderlab_NativeBridge_resize(JNIEnv *env, jobject thiz, jint width, jint height) {
    app->resize(width , height);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_xinlan_shaderlab_NativeBridge_render(JNIEnv *env, jobject thiz, jlong delta_time) {
    app->render(delta_time);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_xinlan_shaderlab_NativeBridge_destory(JNIEnv *env, jobject thiz) {
    app->destory();
    delete app;
    app = nullptr;
}