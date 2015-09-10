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
#include <glm/vec3.hpp>
#include <rapidjson/document.h>
#include <rapidxml/rapidxml.hpp>

std::vector<std::string> getSubDirectories(std::string baseDir);

std::vector<std::string> getFilesList(std::string dir, std::string filter);

glm::vec3 getVec3(rapidjson::Value& val);

std::string remove_extension(const std::string& filename);

rapidxml::xml_node<>* searchByAttribute(rapidxml::xml_node<> *node, const char* attribute, std::string value, const char* nodeName = 0);

void split(std::string s, std::string delimiter, std::vector<std::string> *container);

#endif	/* UTILITY_H */

