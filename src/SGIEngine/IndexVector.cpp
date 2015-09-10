/*
 * File:   IndexVector.cpp
 * Author: TheArni
 *
 * Created on 14 de julio de 2015, 12:45
 */

#include "IndexVector.h"

IndexVector::IndexVector(int pos, int nor, int uv) {
    posVector = pos;
    normalVector = nor;
    uvVector = uv;
}

IndexVector::IndexVector(const IndexVector& orig) {

}

IndexVector::~IndexVector() {

}

