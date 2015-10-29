#include "Utility.h"

#include "definitions.h"

#include <assert.h>
#include <iostream>
#include <fstream>
#include <regex>

#if defined WINDOWS
#include <windows.h>
#include <vector>
#elif defined LINUX
#include <dirent.h>
#include <sys/stat.h>
#endif

#ifdef LINUX
std::vector<std::string> LINUXgetDirectoryContents(std::string baseDir, bool dirMode) {
    std::vector<std::string> ents;
    
    DIR *dStream = opendir(baseDir.c_str());
    
    if (dStream == NULL) {
        return ents;
    }
    
    struct dirent *ent;
    while ((ent = readdir(dStream)) != NULL) {
        std::string str(ent->d_name);
        
        if (str == "." || str == "..") continue;
        
        struct stat st;
        if (fstatat(dirfd(dStream), str.c_str(), &st, 0) < 0) {
            std::cout << "Error (" << __FILE__ << ":" << __LINE__ << ")" << std::endl;
            break;
        }
        
        if (S_ISDIR(st.st_mode) == dirMode) {
            ents.push_back(str);
        }
    }
    closedir(dStream);
    
    return ents;
}
#endif

#ifdef WINDOWS
std::vector<std::string> WINDOWSgetDirectoryContents(std::string baseDir, bool dirMode) {
    std::vector<std::string> result;
    WIN32_FIND_DATA files;
    HANDLE handle = FindFirstFileA((baseDir + "/*").c_str(), &files);

    if (handle != INVALID_HANDLE_VALUE) {
        do {
            if (strcmp(files.cFileName, ".") && strcmp(files.cFileName, "..") && (files.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY) == dirMode) {
                result.push_back(files.cFileName);
            }
        } while (FindNextFileA(handle, &files));
    }
    return result;
}
#endif
std::vector<std::string> getSubDirectories(std::string baseDir) {
#if defined WINDOWS
    return WINDOWSgetDirectoryContents(baseDir, true);
#elif defined LINUX
    return LINUXgetDirectoryContents(baseDir, true);
#else
#error unknown platform
#endif
}

std::vector<std::string> getFilesList(std::string dir, std::string filter) {
    std::vector<std::string> contents;
#if defined WINDOWS
    contents = WINDOWSgetDirectoryContents(dir, false);
#elif defined LINUX
    contents = LINUXgetDirectoryContents(dir, false);
#else
#error unknown platform    
#endif
    std::vector<std::string> results;
    std::smatch m;
    std::regex reg(filter);
    for (std::string str : contents) {
        if (std::regex_search(str, m, reg)) {
            results.push_back(str);
        }
    }
    return results;
}

glm::vec3 getVec3(rapidjson::Value& val) {
    assert(val.IsArray());
    return glm::vec3(val[0u].GetDouble(), val[1u].GetDouble(), val[2u].GetDouble());
}

glm::vec4 getVec4(rapidxml::xml_node<> *val){
    std::vector<std::string> container;
    split(val->value(), " ", &container);
    return glm::vec4(std::stof(container[0]), std::stof(container[1]), std::stof(container[2]), std::stof(container[3]));
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