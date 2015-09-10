/*
 * File:   Shader.h
 * Author: TheArni
 *
 * Created on 12 de abril de 2015, 12:02
 */

#ifndef SHADER_H
#define	SHADER_H

#include <string>
#include <map>

class ShaderProgram {
public:
    ShaderProgram(std::string file);

    void link();

    bool hasUniform();

    int getUniform(std::string name);

    unsigned int getProgramID() {
        return programID;
    }

private:
    std::map<std::string, int> uniforms;
    unsigned int programID;
};

void addShader(std::string name, ShaderProgram* program);

ShaderProgram* getShader(std::string name);

#endif	/* SHADER_H */

