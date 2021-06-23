//
// Created by panyi on 2021/6/23.
//

#ifndef RECORDGLACTIVITY_SHADER_H
#define RECORDGLACTIVITY_SHADER_H

#include "glsl.h"
#include <string>
#include <unordered_map>

//编译shader源码
static GLuint CompileShader(GLenum shaderType, const char* shaderSource);

//创建完整Shader
static GLuint CreateGPUProgram(const char* vsShaderSource, const char* fsShaderSource);

//读取文本文件
//static std::string ReadFileAsText(std::string path);

//从文件中创建shader
//static GLuint CreateGPUProgramFromFile(std::string vertexShaderPath , std::string fragShaderPath);

class Shader{
public:
    //static Shader buildGPUProgramFromFile(std::string vertexShaderPath , std::string fragShaderPath);

    static Shader buildGPUProgram(std::string vertexShaderSrc , std::string fragShaderSrc);

    //使用shader
    void useShader();

    //
    void setUniformInt(std::string key , int value);

    void setUniformFloat(std::string key , float value);

    //设置三维向量
    void setUniformVec3(std::string key , float x , float y , float z);

    //设置二维向量
    void setUniformVec2(std::string key , float x , float y);

    void setUniformBool(std::string key , bool value);

    int findUniformLocation(std::string key);

    //get programid
    inline unsigned int getProgramId(){
        return programId;
    }

protected:
    unsigned int programId;
    std::unordered_map<std::string , int> unifromLocs; //unifrom变量loccation
};

#endif //RECORDGLACTIVITY_SHADER_H
