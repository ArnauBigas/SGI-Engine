/*
 * File:   Utility.h
 * Author: TheArni
 *
 * Created on 29 de junio de 2015, 13:44
 */

#ifndef UTILITY_H
#define	UTILITY_H

#include <string>
#include <vector>
#include <vec3.hpp>
#include <vec4.hpp>
#include <document.h>
#include <rapidxml.hpp>
#include <mat4x4.hpp>

std::vector<std::string> getSubDirectories(std::string baseDir);

std::vector<std::string> getFilesList(std::string dir, std::string filter);

glm::vec3 getVec3(rapidjson::Value& val);

glm::vec4 getVec4(rapidxml::xml_node<> *val);

std::string remove_extension(const std::string& filename);

rapidxml::xml_node<>* searchByAttribute(rapidxml::xml_node<> *node, const char* attribute, std::string value, const char* nodeName = 0);

void split(std::string s, std::string delimiter, std::vector<std::string> *container);

bool readJsonFile(std::string dir, rapidjson::Document& doc);

glm::mat4 convertToRightHandedCoords(glm::mat4 matrix);

template <typename T>
inline T random(T min, T max){
    return min + (((T) rand() / (T) RAND_MAX) * (max-min));
}

template <typename T>
inline T lerp(T v0, T v1, T t) {
    return fma(t, v1, fma(-t, v0, v0));
}

#endif	/* UTILITY_H */

