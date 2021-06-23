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
        std::string vertexSrc = GLSL(
            layout(location = 0) in vec3 aPos;
                    void main(){
                        gl_Position = vec4(aPos.xyz, 1.0);
                    }
            );

        std::string fragSrc = GLSL(
                precision highp float; //
                out vec4 fragColor;

                uniform float iTime;
                uniform vec2 iResolution;

//                vec4 mainImage(vec2 fragCoord){
//                    vec4 fragColor;
//                    vec2 uv = fragCoord/iResolution.xy;
//
//                    // Time varying pixel color
//                    vec3 col = 0.5 + 0.5*cos(iTime+uv.xyx+vec3(0,2,4));
//
//                    // Output to screen
//                    fragColor = vec4(col,1.0);
//
//                    return fragColor;
//                }

                  vec3 palette(float d){
                      return mix(vec3(0.2,0.7,0.9),vec3(1.,0.,1.),d);
                  }

                  vec2 rotate(vec2 p,float a){
                      float c = cos(a);
                      float s = sin(a);
                      return p*mat2(c,s,-s,c);
                  }

                  float map(vec3 p){
                      for( int i = 0; i<8; ++i){
                          float t = iTime*0.2;
                          p.xz =rotate(p.xz,t);
                          p.xy =rotate(p.xy,t*1.89);
                          p.xz = abs(p.xz);
                          p.xz-=.5;
                      }
                      return dot(sign(p),p)/5.;
                  }

                  vec4 rm (vec3 ro, vec3 rd){
                      float t = 0.;
                      vec3 col = vec3(0.);
                      float d;
                      for(float i =0.; i<64.; i++){
                          vec3 p = ro + rd*t;
                          d = map(p)*.5;
                          if(d<0.02){
                              break;
                          }
                          if(d>100.){
                              break;
                          }
                          //col+=vec3(0.6,0.8,0.8)/(400.*(d));
                          col+=palette(length(p)*.1)/(400.*(d));
                          t+=d;
                      }
                      return vec4(col,1./(d*100.));
                  }
                  void mainImage( out vec4 fragColor, in vec2 fragCoord )
                  {
                      vec2 uv = (fragCoord-(iResolution.xy/2.))/iResolution.x;
                      vec3 ro = vec3(0.,0.,-50.);
                      ro.xz = rotate(ro.xz,iTime);
                      vec3 cf = normalize(-ro);
                      vec3 cs = normalize(cross(cf,vec3(0.,1.,0.)));
                      vec3 cu = normalize(cross(cf,cs));

                      vec3 uuv = ro+cf*3. + uv.x*cs + uv.y*cu;

                      vec3 rd = normalize(uuv-ro);

                      vec4 col = rm(ro,rd);

                      fragColor = col;
                  }

              void main(){
                  mainImage(fragColor , gl_FragCoord.xy);
              }
          );

        shader = Shader::buildGPUProgram(vertexSrc , fragSrc);

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