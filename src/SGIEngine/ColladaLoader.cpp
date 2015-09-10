/* 
 * File:   ColladaLoader.cpp
 * Author: arnau
 * 
 * Created on 4 / de setembre / 2015, 19:47
 */

#include "ColladaLoader.h"

#include <iostream>
#include <rapidxml_utils.hpp>
#include <rapidxml.hpp>
#include <string>
#include <vector>
#include <GL/glew.h>

#include "Utility.h"

/*TODO: BENCHMARKS! Is it better to issue different draw calls for each model
 * or maybe you should do some fancy shader trick and incorporate different
 * model matrices onto a uniform array or something? 
 **/

//MOAR TODO: MAKE THIS THING NICER, ACCEPTING ALL KINDS OF CRAZY STUFF AND REMOVE
//ALL THIS HARDCODED NONSENSE

GLenum readData(std::vector<float> *container, rapidxml::xml_node<> *node){
    if(node->first_node("float_array") != 0){
        std::vector<std::string> tempContainer;
        split(node->first_node("float_array")->value(), " ", &tempContainer);
        for(std::string s : tempContainer){
            container->push_back(std::stof(s));
        }
        return GL_FLOAT;
    } else {
        std::cerr << "Source data type not currently supported by the loader! Sorry, working on it D:" << std::endl;
    }
    return GL_NONE;
}

Model* loadColladaModel(std::string filename){
    rapidxml::file<> xmlFile(filename.c_str());
    rapidxml::xml_document<> doc;
    doc.parse<0>(xmlFile.data());
    rapidxml::xml_node<> *geometries = doc.first_node()->first_node("library_geometries");
    if(geometries->first_node("geometry") == 0){
        std::cerr << "Couldn't load model: file contains no geometry data." << std::endl;
        return 0;
    }
    std::vector<float> openglData;
    std::vector<unsigned int> indices;
    unsigned int indexOffset = 0;
    for(rapidxml::xml_node<> *geometry = geometries->first_node("geometry"); geometry != 0; geometry = geometry->next_sibling("geometry")){
        std::cout << "Loading geometry with id: " << geometry->first_attribute("id")->value() << std::endl;
        rapidxml::xml_node<> *mesh = geometry->first_node("mesh");
        if(mesh == 0){
            std::cerr << "Couldn't load model: The loader can only load geometry of type \"mesh\" (geometry with name: " << geometry->first_attribute("name")->value() << ")." << std::endl;
            return 0;
        }
        rapidxml::xml_node<> *polylist = mesh->first_node("polylist");
        if(polylist == 0){
            std::cerr << "Couldn't load model: The loader can only load meshes that use polylists. (geometry with name: " << geometry->first_attribute("name")->value() << ")." << std::endl;
            return 0;
        }
        //TODO: allow for multiple data types
        std::vector<float> vertexData;
        GLenum vertexDataType;
        unsigned int vertexOffset;
        unsigned int vertexStride;
        std::vector<float> normalData;
        GLenum normalDataType;
        unsigned int normalOffset;
        unsigned int normalStride;
        std::vector<float> uvData;
        GLenum uvDataType;
        unsigned int uvOffset;
        unsigned int uvStride;
        for(rapidxml::xml_node<> *input = polylist->first_node("input"); input != 0; input = input->next_sibling("input")){
            std::string semantic(input->first_attribute("semantic")->value());
            if(semantic == "VERTEX"){
                vertexDataType = readData(&vertexData, searchByAttribute(mesh, "id", std::string(mesh->first_node("vertices")->first_node("input")->first_attribute("source")->value()).substr(1), "source"));
                if(vertexDataType == GL_NONE){
                    return 0;
                }
                vertexOffset = std::stoi(input->first_attribute("offset")->value());
                vertexStride = std::stoi(searchByAttribute(mesh, "id", std::string(mesh->first_node("vertices")->first_node("input")->first_attribute("source")->value()).substr(1), "source")->first_node("technique_common")->first_node("accessor")->first_attribute("stride")->value());
            } else if(semantic == "NORMAL"){
                normalDataType = readData(&normalData, searchByAttribute(mesh, "id", std::string(input->first_attribute("source")->value()).substr(1), "source"));
                if(normalDataType == GL_NONE){
                    return 0;
                }
                normalOffset = std::stoi(input->first_attribute("offset")->value());
                normalStride = std::stoi(searchByAttribute(mesh, "id", std::string(input->first_attribute("source")->value()).substr(1), "source")->first_node("technique_common")->first_node("accessor")->first_attribute("stride")->value());
            } else if(semantic == "TEXCOORD"){
                uvDataType = readData(&uvData, searchByAttribute(mesh, "id", std::string(input->first_attribute("source")->value()).substr(1), "source"));
                if(uvDataType == GL_NONE){
                    return 0;
                }
                uvOffset = std::stoi(input->first_attribute("offset")->value());
                uvStride = std::stoi(searchByAttribute(mesh, "id", std::string(input->first_attribute("source")->value()).substr(1), "source")->first_node("technique_common")->first_node("accessor")->first_attribute("stride")->value());
            } else {
                std::cerr << "Unknown input semantic: " << semantic << std::endl;
                return 0;
            }
        }
        std::vector<std::string> tempContainer;
        std::vector<int> colladaIndices;        
        split(polylist->first_node("p")->value(), " ", &tempContainer);
        for(std::string s : tempContainer){
            colladaIndices.push_back(std::stoi(s));
        }
        //Generate vertices
        for(unsigned int i = 0; i < std::stoi(polylist->first_attribute("count")->value())*3; i++){
            openglData.push_back(vertexData.at(colladaIndices.at(i*3+vertexOffset)*vertexStride));
            openglData.push_back(vertexData.at(colladaIndices.at(i*3+vertexOffset)*vertexStride+1));
            openglData.push_back(vertexData.at(colladaIndices.at(i*3+vertexOffset)*vertexStride+2));
            
            openglData.push_back(normalData.at(colladaIndices.at(i*3+normalOffset)*normalStride));
            openglData.push_back(normalData.at(colladaIndices.at(i*3+normalOffset)*normalStride+1));
            openglData.push_back(normalData.at(colladaIndices.at(i*3+normalOffset)*normalStride+2));
            
            openglData.push_back(uvData.at(colladaIndices.at(i*3+uvOffset)*uvStride));
            openglData.push_back(uvData.at(colladaIndices.at(i*3+uvOffset)*uvStride+1));
            
            //temp index
            indices.push_back(indexOffset+i);
        }
        indexOffset += std::stoi(polylist->first_attribute("count")->value())*3;
    }
    return new Model(openglData, indices);
}