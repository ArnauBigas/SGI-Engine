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
    ShaderProgram(std::string shaderRoot, std::string file);

    void link();

    bool hasUniform(std::string name);

    /**
     * Doesn't actually check if it's a valid uniform, use hasUniform() for that.
     * @param name The name of the uniform you want the id of.
     * @return the id of the uniform (to be used with opengl methods)
     */
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

