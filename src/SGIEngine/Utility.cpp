#include "Utility.h"

#include "definitions.h"

#include <assert.h>
#include <iostream>
#include <fstream>

#if defined WINDOWS
#include <windows.h>
#endif

std::vector<std::string> getSubDirectories(std::string baseDir) {
    std::vector<std::string> result;
#if defined WINDOWS
    WIN32_FIND_DATA files;
    HANDLE handle = FindFirstFileA((baseDir + "/*").c_str(), &files);

    if (handle != INVALID_HANDLE_VALUE) {
        do {
            if (strcmp(files.cFileName, ".") && strcmp(files.cFileName, "..") && files.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                result.push_back(files.cFileName);
            }
        } while (FindNextFileA(handle, &files));
    }
#elif defined LINUX

#else
#error unknown platform
#endif
    return result;
}

std::vector<std::string> getFilesList(std::string dir, std::string filter) {
    std::vector<std::string> result;
#if defined WINDOWS
    WIN32_FIND_DATA files;
    HANDLE handle = FindFirstFileA((dir + filter).c_str(), &files);

    if (handle != INVALID_HANDLE_VALUE) {
        do {
            result.push_back(files.cFileName);
        } while (FindNextFileA(handle, &files));
    }
#elif defined LINUX

#else
#error unknown platform    
#endif
    return result;
}

glm::vec3 getVec3(rapidjson::Value& val) {
    assert(val.IsArray());
    return glm::vec3(val[0u].GetDouble(), val[1u].GetDouble(), val[2u].GetDouble());
}

std::string remove_extension(const std::string& filename) {
    size_t lastdot = filename.find_last_of(".");
    if (lastdot == std::string::npos) return filename;
    return filename.substr(0, lastdot);
}

rapidxml::xml_node<>* searchByAttribute(rapidxml::xml_node<> *node, const char* attribute, std::string value, const char* nodeName){
    for(rapidxml::xml_node<> *result = node->first_node(nodeName); result != 0; result = result->next_sibling(nodeName)){
        if(std::string(result->first_attribute(attribute)->value()) == value){
            return result;
        }
    }
    return 0;
}

void split(std::string s, std::string delimiter, std::vector<std::string> *container){
    std::string::size_type i = 0;
    std::string::size_type j = s.find(delimiter);

    while (j != std::string::npos) {
          container->push_back(s.substr(i, j-i));
          i = ++j;
          j = s.find(delimiter, j);

          if (j == std::string::npos)
             container->push_back(s.substr(i, s.length()));
    }
}

bool readJsonFile(std::string dir, rapidjson::Document& doc){
    std::ifstream stream(dir, std::ios::in);
    if (stream.is_open()) {
        std::string content((std::istreambuf_iterator<char>(stream)),
                       (std::istreambuf_iterator<char>()));
        rapidjson::StringStream ss(content.c_str());
        doc.ParseStream(ss);
        return true;
    } else {
        return false;
    }    
}