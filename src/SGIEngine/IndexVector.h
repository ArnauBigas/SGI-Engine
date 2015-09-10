/*
 * File:   IndexVector.h
 * Author: TheArni
 *
 * Created on 14 de julio de 2015, 12:45
 */

#ifndef INDEXVECTOR_H
#define	INDEXVECTOR_H

#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

class IndexVector {
public:
    IndexVector(int pos, int nor, int uv);
    IndexVector(const IndexVector& orig);
    virtual ~IndexVector();

    bool equals(IndexVector* vec) {
        return vec->posVector == this->posVector && vec->normalVector == this->normalVector && vec->uvVector == this->uvVector;
    }

    std::vector<int> getSimilarVertices() {
        return similar;
    }

    void uploadData(std::vector<float> *buffer, std::vector<glm::vec3> *posVectors, std::vector<glm::vec3> *normalVectors, std::vector<glm::vec2> *uvVectors) {
        buffer->push_back(posVectors->at(posVector).x);
        buffer->push_back(posVectors->at(posVector).y);
        buffer->push_back(posVectors->at(posVector).z);

        buffer->push_back(normalVectors->at(normalVector).x);
        buffer->push_back(normalVectors->at(normalVector).y);
        buffer->push_back(normalVectors->at(normalVector).z);

        buffer->push_back(uvVectors->at(uvVector).x);
        buffer->push_back(uvVectors->at(uvVector).y);
    }

    int posVector;
    int normalVector;
    int uvVector;

private:
    std::vector<int> similar;
};

#endif	/* INDEXVECTOR_H */

