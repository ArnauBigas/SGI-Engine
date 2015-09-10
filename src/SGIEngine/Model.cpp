/*
 * File:   Model.cpp
 * Author: TheArni
 *
 * Created on 25 de junio de 2015, 23:05
 */

#include "Model.h"

#include <cstring>
#include <stdio.h>
#include <vec3.hpp>
#include <vec2.hpp>
#include <map>
#include <GL/glew.h>
#include <iostream>

#include "Utility.h"
#include "IndexVector.h"
#include "definitions.h"

Model::Model(std::string file) {
    std::string line;
    FILE * f = fopen(file.c_str(), "r");
    std::vector<glm::vec3> positionVectors;
    std::vector<glm::vec2> uvVectors;
    std::vector<glm::vec3> normalVectors;
    std::map<int, IndexVector*> vertices;
    std::vector<unsigned int> indices;
    if (f != NULL) {
        //Parse OBJ
        while (true) {
            char lineHeader[64];
            int res = fscanf(f, "%s", lineHeader);
            if (res == EOF)
                break;
            if (strcmp(lineHeader, "v") == 0) {
                glm::vec3 vertex;
                fscanf(f, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
                positionVectors.push_back(vertex);
            } else if (strcmp(lineHeader, "vt") == 0) {
                glm::vec2 uv;
                fscanf(f, "%f %f\n", &uv.x, &uv.y);
                uvVectors.push_back(uv);
            } else if (strcmp(lineHeader, "vn") == 0) {
                glm::vec3 normal;
                fscanf(f, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
                normalVectors.push_back(normal);
            } else if (strcmp(lineHeader, "f") == 0) {
                unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
                int matches = fscanf(f, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
                if (matches != 9) {
                    printf("The parser can't load untriangulated models.\n");
                    loaded = false;
                }
                unsigned int ivIndex[3];
                for (int i = 0; i < 3; i++) {
                    IndexVector* iv = new IndexVector(vertexIndex[i] - 1, normalIndex[i] - 1, uvIndex[i] - 1);
                    if (vertices.find(vertexIndex[i] - 1) == vertices.end()) {
                        vertices.insert(std::make_pair(vertexIndex[i] - 1, iv));
                        ivIndex[i] = vertexIndex[i] - 1;
                    } else if (iv->equals(vertices.at(vertexIndex[i] - 1))) {
                        ivIndex[i] = vertexIndex[i] - 1;
                        delete iv;
                    } else {
                        bool found = false;
                        for (int j : vertices.at(vertexIndex[i] - 1)->getSimilarVertices()) {
                            if (iv->equals(vertices.at(j))) {
                                ivIndex[i] = j;
                                found = true;
                                break;
                            }
                        }
                        if (!found) {
                            vertices.at(vertexIndex[i] - 1)->getSimilarVertices().push_back(vertices.size());
                            ivIndex[i] = vertices.size();
                            vertices.insert(std::make_pair(vertices.size(), iv));
                        } else {
                            delete iv;
                        }
                    }
                }
                indices.push_back(ivIndex[0]);
                indices.push_back(ivIndex[1]);
                indices.push_back(ivIndex[2]);
            } else {
                /*printf("Couldn't recognize line header: ");
                printf(lineHeader);
                printf("\n");*/
            }
        }
        //Generate object data
        std::vector<float> data;
        for (int i = 0; i < vertices.size(); i++) {
            vertices.at(i)->uploadData(&data, &positionVectors, &normalVectors, &uvVectors);
        }

        sendToOpengl(data, indices);

        fclose(f);

        //Load successful!
        elements = indices.size()*3;
        loaded = true;
        std::cout << "model successfully loaded (" << indices.size() << " indices)" << std::endl;
    } else {
        printf("File couldn't be opened.\n");
        loaded = false;
    }
}

Model::Model(std::vector<float> data, std::vector<unsigned int> indices) {
    sendToOpengl(data, indices);
    elements = indices.size()*3;
    loaded = true;
}

Model::Model(const Model & orig) {
}

Model::~Model() {
}

void Model::render() {
    glBindVertexArray(vaoid);
    glDrawElements(GL_TRIANGLES, elements, GL_UNSIGNED_INT, 0); //Draw everything

}

void Model::sendToOpengl(std::vector<float> data, std::vector<unsigned int> indices) {
    //Generate OpenGL buffers
    glGenVertexArrays(1, &vaoid);
    glGenBuffers(1, &vboid);
    glGenBuffers(1, &iboid);

    //Bind buffers and upload data to OpenGL
    glBindVertexArray(vaoid);

    glBindBuffer(GL_ARRAY_BUFFER, vboid); //Bind VBO
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof (float), &data[0], GL_STATIC_DRAW); //Upload Vertex Data

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboid); //Bind IBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof (unsigned int), &indices[0], GL_STATIC_DRAW); //Upload Index

    //Setup attrib pointers
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 32, 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 32, BUFFER_OFFSET(12));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 32, BUFFER_OFFSET(24));
}
