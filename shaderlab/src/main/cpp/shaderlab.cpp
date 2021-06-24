//
// Created by panyi on 2021/6/23.
//

#include <jni.h>
#include <string>
#include "glsl.h"
#include "shader.h"

class App{
public:
    void init(){
        vertSrc = GLSL(
            layout(location = 0) in vec3 aPos;
                    void main(){
                        gl_Position = vec4(aPos.xyz, 1.0);
                    }
             );

        std::string mainCode = GLSL_CODE(
                vec4 mainImage( out vec4 fragColor, in vec2 fragCoord){
                   vec2 uv = fragCoord/iResolution.xy;
                   // Time varying pixel color
                   vec3 col = 0.5 + 0.5*cos(iTime+uv.xyx+vec3(0,2,4));
                   // Output to screen
                   fragColor = vec4(1.0 ,0.0 , 0.0,1.0);
                   return fragColor;
               }
        );

        //sprintf("" ,fragCodeTemplate.c_str() , "helloworld()");
        //shader = Shader::buildGPUProgram(vertSrc , fragSrc);
        resetFragShaderCode(mainCode);

        glGenVertexArrays(1 , &vao);
        glGenBuffers(1 , &vbo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER , vbo);
        glBufferData(GL_ARRAY_BUFFER , sizeof(vertices) , vertices , GL_STATIC_DRAW);

        glVertexAttribPointer(0 , 3 , GL_FLOAT , GL_FALSE , 3 * sizeof(float) , (void *)0);
        glEnableVertexAttribArray(0);
    }

    void resize(int w , int h){
        this->width = w;
        this->height = h;
        init();
    }

    void render(long deltatime){
        time += (deltatime / 1000.0f);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //LOGE("shaderID = %d" , shader.getProgramId());
        shader.useShader();
        shader.setUniformFloat("iTime", time);
        shader.setUniformVec2("iResolution" , width , height);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES , 0 , 6);
    }

    void destory(){
        glDeleteProgram(shader.getProgramId());

        glDeleteBuffers(1 , &vbo);
        glDeleteVertexArrays(1 , &vao);
    }

    void resetFragShaderCode(std::string mainCode){
        fragSrc = string_format(fragTemplate , mainCode.c_str());
        LOGE("fragSrc = %s \n" , fragSrc.c_str());
        shader = Shader::buildGPUProgram(vertSrc , fragSrc);
    }

    std::string jstring2string(JNIEnv *env, jstring jStr) {
        if (!jStr)
            return "";

        const jclass stringClass = env->GetObjectClass(jStr);
        const jmethodID getBytes = env->GetMethodID(stringClass, "getBytes", "(Ljava/lang/String;)[B");
        const jbyteArray stringJbytes = (jbyteArray) env->CallObjectMethod(jStr, getBytes, env->NewStringUTF("UTF-8"));

        size_t length = (size_t) env->GetArrayLength(stringJbytes);
        jbyte* pBytes = env->GetByteArrayElements(stringJbytes, NULL);

        std::string ret = std::string((char *)pBytes, length);
        env->ReleaseByteArrayElements(stringJbytes, pBytes, JNI_ABORT);

        env->DeleteLocalRef(stringJbytes);
        env->DeleteLocalRef(stringClass);
        return ret;
    }

    std::string string_format(const std::string fmt_str, ...) {
        int final_n, n = ((int)fmt_str.size()) * 2; /* Reserve two times as much as the length of the fmt_str */
        std::unique_ptr<char[]> formatted;
        va_list ap;
        while(1) {
            formatted.reset(new char[n]); /* Wrap the plain char array into the unique_ptr */
            strcpy(&formatted[0], fmt_str.c_str());
            va_start(ap, fmt_str);
            final_n = vsnprintf(&formatted[0], n, fmt_str.c_str(), ap);
            va_end(ap);
            if (final_n < 0 || final_n >= n)
                n += abs(final_n - n + 1);
            else
                break;
        }
        return std::string(formatted.get());
    }
private:

    float vertices[3 * 6] = {
            -1.0f , 1.0f ,0.0f ,
            -1.0f , -1.0f , 0.0f ,
            1.0f , 1.0f,  0.0f ,
            1.0f , 1.0f ,0.0f ,
            -1.0f , -1.0f , 0.0f ,
            1.0f , -1.0f,  0.0f ,
    };

    Shader shader;

    float width;
    float height;

    unsigned int vao;
    unsigned int vbo;

    float time;

    std::string vertSrc;
    std::string fragSrc;

    std::string fragTemplate = GLSL(
     precision highp float; //
          out vec4 fragColor;
          uniform float iTime;
          uniform vec2 iResolution;

          %s

          void main(){
              mainImage(fragColor , gl_FragCoord.xy);
          }
     );
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

extern "C"
JNIEXPORT void JNICALL
Java_com_xinlan_shaderlab_NativeBridge_compileAndRun(JNIEnv *env, jobject thiz,
                                                     jstring main_image_src) {
    if(app != nullptr){
        std::string src = app->jstring2string(env , main_image_src);
        app->resetFragShaderCode(src);
    }
}