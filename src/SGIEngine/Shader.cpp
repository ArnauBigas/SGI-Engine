#include "Shader.h"

#include <GL/glew.h>
#include <vector>
#include <fstream>
#include <document.h>

#include "Utility.h"
#include "definitions.h"
#include "Logger.h"

std::string* getSource(const char* path) {
    std::string* source = new std::string();
    std::ifstream stream(path, std::ios::in);
    if (stream.is_open()) {
        std::string line = "";
        while (std::getline(stream, line))
            (*source) += "\n" + line;
        stream.close();
    }
    return source;
}

bool compileShader(unsigned int shaderID, const char* source) {
    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Compile Vertex Shader
    glShaderSource(shaderID, 1, &source, NULL);
    glCompileShader(shaderID);

    // Check Vertex Shader
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> VertexShaderErrorMessage(InfoLogLength);
    glGetShaderInfoLog(shaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
    if (Result == GL_FALSE) {
        fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);
    }
    return Result == GL_TRUE;
}

unsigned int loadProgram(const char* vertexShaderFile, const char* fragmentShaderFile) {
    unsigned int vertexID = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentID = glCreateShader(GL_FRAGMENT_SHADER);

    std::string* vertexShaderCode = getSource(vertexShaderFile);

    std::string* fragmentShaderCode = getSource(fragmentShaderFile);

    if (!compileShader(vertexID, vertexShaderCode->c_str())) {
        return -1;
    }

    if (!compileShader(fragmentID, fragmentShaderCode->c_str())) {
        return -1;
    }

    delete(vertexShaderCode);
    delete(fragmentShaderCode);

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Link the program
    unsigned int programID = glCreateProgram();
    glAttachShader(programID, vertexID);
    glAttachShader(programID, fragmentID);
    glLinkProgram(programID);

    // Check the program
    glGetProgramiv(programID, GL_LINK_STATUS, &Result);
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> ProgramErrorMessage(std::max(InfoLogLength, int(1)));
    glGetProgramInfoLog(programID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
    if (Result == GL_FALSE) {
        fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);
    }

    glDeleteShader(vertexID);
    glDeleteShader(fragmentID);

    if (Result == GL_FALSE) {
        return -1;
    } else {
        return programID;
    }
}

ShaderProgram::ShaderProgram(std::string file) {
    rapidjson::Document doc;
    if(readJsonFile(file, doc)){
        programID = loadProgram((FSHADERS + std::string(doc["vertexShader"].GetString())).c_str(), (FSHADERS + std::string(doc["fragmentShader"].GetString())).c_str());
        link();
        rapidjson::Value& uniforms = doc["uniforms"];
        for (rapidjson::SizeType i = 0; i < uniforms.Size(); i++) {
            std::string uniform = uniforms[i].GetString();
            int location = glGetUniformLocation(programID, uniform.c_str());
            this->uniforms.insert(std::make_pair(uniform, location));
            if(uniform == "diffuseBuffer"){
                glUniform1i(location, DIFFUSEBUFFERTEXTUREUNIT);
            } else if(uniform == "normalBuffer"){
                glUniform1i(location, NORMALBUFFERTEXTUREUNIT);
            } else if(uniform == "materialBuffer"){
                glUniform1i(location, MATERIALBUFFERTEXTUREUNIT);
            } else if(uniform == "depthBuffer"){
                glUniform1i(location, DEPTHBUFFERTEXTUREUNIT);
            } else if(uniform == "diffuseTexture"){
                glUniform1i(location, DIFFUSETEXTUREUNIT);
            } else if(uniform == "normalTexture"){
                glUniform1i(location, NORMALMAPTEXTUREUNIT);
            } else if(uniform == "specularTexture"){
                glUniform1i(location, SPECULARMAPTEXTUREUNIT);
            } else if(uniform == "shadowmapTexture"){
                glUniform1i(location, SHADOWMAPTEXTUREUNIT);
            } else if(uniform == "guiTexture"){
                glUniform1i(location, GUITEXTUREUNIT);
            } else if(uniform == "noiseTexture"){
                glUniform1i(location, RANDOMNOISETEXTUREUNIT);
            } else if(uniform == "ssaoTexture"){
                glUniform1i(location, SSAOTEXTUREUNIT);
            }
        }
    }
}

void ShaderProgram::link() {
    glUseProgram(programID);
}

int ShaderProgram::getUniform(std::string name) {
    return uniforms[name];
}

bool ShaderProgram::hasUniform(std::string name){
    return uniforms.find(name) != uniforms.end();
}

std::map<std::string, ShaderProgram*> programs;

void addShader(std::string name, ShaderProgram* program) {
    programs.insert(std::make_pair(name, program));
}

ShaderProgram* getShader(std::string name) {
    if(programs.find(name) == programs.end()){
        Logger::info << "Couldn't find shader program " << name << std::endl;
        return 0;
    }
    return programs.at(name);
}