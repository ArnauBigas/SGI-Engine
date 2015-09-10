/*
 * File:   Model.h
 * Author: TheArni
 *
 * Created on 25 de junio de 2015, 23:05
 */

#ifndef MODEL_H
#define	MODEL_H

#include <string>
#include <vector>

class Model {
public:
    Model(std::string file);
    Model(std::vector<float> data, std::vector<unsigned int> indices);
    Model(const Model& orig);
    virtual ~Model();

    bool success() {
        return loaded;
    }

    void render();

private:
    bool loaded = false;
    unsigned int vboid;
    unsigned int iboid;
    unsigned int vaoid;
    int elements;

    void sendToOpengl(std::vector<float> data, std::vector<unsigned int> indices);
};

#endif	/* MODEL_H */

