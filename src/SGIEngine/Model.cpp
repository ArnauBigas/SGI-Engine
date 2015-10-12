/*
 * File:   Model.cpp
 * Author: TheArni
 *
 * Created on 25 de junio de 2015, 23:05
 */

#include "Model.h"

#include <iostream>
#include <rapidxml_utils.hpp>
#include <rapidxml.hpp>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>

#include "Utility.h"
#include "definitions.h"
#include "RenderEngine.h"
#include "Texture.h"

unsigned int sendToOpengl(std::vector<float> data, std::vector<unsigned int> indices) {
    //Generate OpenGL buffers
    unsigned int vaoid;
    unsigned int vboid;
    unsigned int iboid;
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
    
    return vaoid;
}

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

bool Model::loadCollada(std::string filename){
    //Load the collada xml file
    rapidxml::file<> xmlFile(filename.c_str());
    rapidxml::xml_document<> doc;
    doc.parse<0>(xmlFile.data());
    
    
    //Load textures
    rapidxml::xml_node<> *images = doc.first_node()->first_node("library_images");
    if(images->first_node("image") == 0){
        std::cerr << "Couldn't load model: file contains no texture data." << std::endl;
        return false;
    }
    std::map<std::string, unsigned int> textures;
    for(rapidxml::xml_node<> *image = images->first_node("image"); image != 0; image = image->next_sibling("image")){
        textures.insert(std::pair<std::string, unsigned int>(image->first_attribute("id")->value(), loadTextureFromPNG(filename.substr(0, filename.find_last_of("\\/")+1) + image->first_node("init_from")->value())));
    }
    
    
    //Load effects
    //TODO: only supports common profiles using phong techniques
    rapidxml::xml_node<> *effectsNode = doc.first_node()->first_node("library_effects");
    if(effectsNode->first_node("effect") == 0){
        std::cerr << "Couldn't load model: file contains no effects data." << std::endl;
        return false;
    }
    std::map<std::string, Material> effects;
    for(rapidxml::xml_node<> *effect = effectsNode->first_node("effect"); effect != 0; effect = effect->next_sibling("effect")){
        rapidxml::xml_node<> *profile = effect->first_node("profile_COMMON");
        if(profile != 0){
            rapidxml::xml_node<> *technique = profile->first_node("technique");
            Material mat;
            rapidxml::xml_node<> *phong = technique->first_node("phong");
            if(phong != 0){                
                if(phong->first_node("emission") != 0){
                    mat.emission = getVec4(phong->first_node("emission")->first_node("color"));
                }
                if(phong->first_node("ambient") != 0){
                    mat.ambient = getVec4(phong->first_node("ambient")->first_node("color"));
                }
                if(phong->first_node("diffuse") != 0){
                    if(phong->first_node("diffuse")->first_node("color") != 0){
                        mat.diffuse = getVec4(phong->first_node("diffuse")->first_node("color"));
                    }
                    if(phong->first_node("diffuse")->first_node("texture") != 0){
                        mat.diffuseTexture = textures.at(searchByAttribute(profile, "sid", 
                                searchByAttribute(profile, "sid", phong->first_node("diffuse")->first_node("texture")->first_attribute("texture")->value(), "newparam")
                                ->first_node("sampler2D")->first_node("source")->value()
                                , "newparam")->first_node("surface")->first_node("init_from")->value());
                    }
                }
                if(phong->first_node("specular") != 0){
                    if(phong->first_node("specular")->first_node("color") != 0){
                        mat.specular = getVec4(phong->first_node("specular")->first_node("color"));
                    }
                    if(phong->first_node("specular")->first_node("texture") != 0){
                        mat.specularTexture = textures.at(searchByAttribute(profile, "sid", 
                                searchByAttribute(profile, "sid", phong->first_node("specular")->first_node("texture")->first_attribute("texture")->value(), "newparam")
                                ->first_node("sampler2D")->first_node("source")->value()
                                , "newparam")->first_node("surface")->first_node("init_from")->value());
                    }
                }
                if(phong->first_node("shininess") != 0){
                    mat.shininess = std::stof(phong->first_node("shininess")->first_node("float")->value());
                }
            } else {
                std::cerr << "Couldn't load model: material doesn't use the phong technique." << std::endl;
                return false;
            }
            if(profile->first_node("extra") != 0 && profile->first_node("extra")->first_node("technique")->first_node("bump") != 0){
                mat.bumpmapTexture = textures.at(searchByAttribute(profile, "sid", 
                                searchByAttribute(profile, "sid", profile->first_node("extra")->first_node("technique")->first_node("bump")->first_node("texture")->first_attribute("texture")->value(), "newparam")
                                ->first_node("sampler2D")->first_node("source")->value()
                                , "newparam")->first_node("surface")->first_node("init_from")->value());
            }
            effects.insert(std::pair<std::string, Material>(effect->first_attribute("id")->value(), mat));
        } else {
            std::cerr << "Couldn't load model: material doesn't have a profile_COMMON tag." << std::endl;
            return false;
        }
    }
    
    
    //Load materials
    //TODO: make them overridable (collada spec))
    rapidxml::xml_node<> *materialsNode = doc.first_node()->first_node("library_materials");
    if(materialsNode->first_node("material") == 0){
        std::cerr << "Couldn't load model: file contains no material data." << std::endl;
        return false;
    }
    std::map<std::string, Material> materials;
    for(rapidxml::xml_node<> *material = materialsNode->first_node("material"); material != 0; material = material->next_sibling("material")){
        materials.insert(std::pair<std::string, Material>(material->first_attribute("id")->value(), effects.at(std::string(material->first_node("instance_effect")->first_attribute("url")->value()).substr(1))));
    }
    
    
    //Load geometry
    std::map<std::string, Mesh> meshes;
    rapidxml::xml_node<> *geometries = doc.first_node()->first_node("library_geometries");
    if(geometries->first_node("geometry") == 0){
        std::cerr << "Couldn't load model: file contains no geometry data." << std::endl;
        return false;
    }
    for(rapidxml::xml_node<> *geometry = geometries->first_node("geometry"); geometry != 0; geometry = geometry->next_sibling("geometry")){
        std::cout << "Loading geometry with id: " << geometry->first_attribute("id")->value() << std::endl;
        Mesh mesh;        
        rapidxml::xml_node<> *meshNode = geometry->first_node("mesh");
        if(meshNode == 0){
            std::cerr << "Couldn't load model: The loader can only load geometry of type \"mesh\" (geometry with name: " << geometry->first_attribute("name")->value() << ")." << std::endl;
            return false;
        }
        if(meshNode->first_node("polylist") == 0){
            std::cerr << "Couldn't load model: The loader can only load meshes that use polylists. (geometry with name: " << geometry->first_attribute("name")->value() << ")." << std::endl;
            return false;
        }
        
        //TODO: allow for multiple data types
        //<name, <stride, data>>
        std::map<std::string, std::pair<unsigned int, std::vector<float>>> sources;
        GLenum dataType;
        for(rapidxml::xml_node<> *source = meshNode->first_node("source"); source != 0; source = source->next_sibling("source")){
            std::vector<float> container;
            dataType = readData(&container, source);
            if(dataType == GL_NONE){
                return false;
            }
            sources.insert(std::pair<std::string, std::pair<unsigned int, std::vector<float>>>(source->first_attribute("id")->value(), std::pair<unsigned int, std::vector<float>>(std::stoi(source->first_node("technique_common")->first_node("accessor")->first_attribute("stride")->value()), container)));
        }
        
        //TODO: same data vertex merging (aka indexing)
        for(rapidxml::xml_node<> *polylist = meshNode->first_node("polylist"); polylist != 0; polylist = polylist->next_sibling("polylist")){
            std::vector<float> openglData;
            std::vector<unsigned int> indices;
            std::vector<std::string> tempContainer;
            std::vector<int> colladaIndices;        
            split(polylist->first_node("p")->value(), " ", &tempContainer);
            for(std::string s : tempContainer){
                colladaIndices.push_back(std::stoi(s));
            }
            std::vector<float> vertexData;
            float vertexOffset;
            float vertexStride;
            std::vector<float> normalData;
            float normalOffset;
            float normalStride;
            std::vector<float> uvData;
            float uvOffset;
            float uvStride;
            
            std::pair<unsigned int, std::vector<float>> pair;
            for(rapidxml::xml_node<> *input = polylist->first_node("input"); input != 0; input = input->next_sibling("input")){
                std::string semantic(input->first_attribute("semantic")->value());
                if(semantic == "VERTEX"){
                    pair = sources.at(std::string(meshNode->first_node("vertices")->first_node("input")->first_attribute("source")->value()).substr(1));
                    vertexData = pair.second;
                    vertexOffset = std::stoi(input->first_attribute("offset")->value());
                    vertexStride = pair.first;
                } else if(semantic == "NORMAL"){
                    pair = sources.at(std::string(input->first_attribute("source")->value()).substr(1));
                    normalData = pair.second;
                    normalOffset = std::stoi(input->first_attribute("offset")->value());
                    normalStride = pair.first;
                } else if(semantic == "TEXCOORD"){
                    pair = sources.at(std::string(input->first_attribute("source")->value()).substr(1));
                    uvData = pair.second;
                    uvOffset = std::stoi(input->first_attribute("offset")->value());
                    uvStride = pair.first;
                } else {
                    std::cerr << "Unknown input semantic: " << semantic << std::endl;
                    return 0;
                }
            }            
            PolyGroup poly;
            poly.elements = std::stoi(polylist->first_attribute("count")->value())*3;
            
            //Generate vertices
            for(unsigned int i = 0; i < poly.elements; i++){
                openglData.push_back(vertexData.at(colladaIndices.at(i*3+vertexOffset)*vertexStride));
                openglData.push_back(vertexData.at(colladaIndices.at(i*3+vertexOffset)*vertexStride+1));
                openglData.push_back(vertexData.at(colladaIndices.at(i*3+vertexOffset)*vertexStride+2));

                openglData.push_back(normalData.at(colladaIndices.at(i*3+normalOffset)*normalStride));
                openglData.push_back(normalData.at(colladaIndices.at(i*3+normalOffset)*normalStride+1));
                openglData.push_back(normalData.at(colladaIndices.at(i*3+normalOffset)*normalStride+2));

                openglData.push_back(uvData.at(colladaIndices.at(i*3+uvOffset)*uvStride));
                //collada stores the uv data the other way around. how silly.
                openglData.push_back(1.0f - uvData.at(colladaIndices.at(i*3+uvOffset)*uvStride+1));

                //temp index
                indices.push_back(i);
            }
            poly.vaoid = sendToOpengl(openglData, indices);
            poly.material = materials.at(polylist->first_attribute("material")->value());
            mesh.addPolyGroup(poly);
        }
        meshes.insert(std::pair<std::string, Mesh>(geometry->first_attribute("id")->value(), mesh));
        //this->meshes.insert(std::pair<std::string, std::pair<glm::mat4, Mesh>>(std::string(geometry->first_attribute("id")->value()).substr(1), std::pair<glm::mat4, Mesh>(glm::mat4(1.0f), mesh)));
    }
    
    //Load scene
    rapidxml::xml_node<> *scenes = doc.first_node()->first_node("library_visual_scenes");
    if(scenes->first_node("visual_scene") == 0){
        std::cerr << "Couldn't load model: file contains no scene data." << std::endl;
        return false;
    } else {
        rapidxml::xml_node<> *scene = scenes->first_node("visual_scene");
        for(rapidxml::xml_node<> *node = scene->first_node("node"); node != 0; node = node->next_sibling("node")){
            if(node->first_node("instance_geometry") != 0){
                std::vector<float> matrix;
                std::vector<std::string> tempContainer;
                split(node->first_node("matrix")->value(), " ", &tempContainer);
                for(std::string s : tempContainer){
                    matrix.push_back(std::stof(s));
                }
                glm::mat4 mat4 = glm::make_mat4(matrix.data());
                if(std::string(doc.first_node()->first_node("asset")->first_node("up_axis")->value()) == "Z_UP"){
                    mat4 = glm::rotate(mat4, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
                }
                this->meshes.insert(std::pair<std::string,std::pair<glm::mat4, Mesh>>(
                        node->first_attribute("id")->value(),
                        std::pair<glm::mat4, Mesh>(
                            mat4,
                            meshes.at(std::string(node->first_node("instance_geometry")->first_attribute("url")->value()).substr(1)))));
            }
        }
    }    
    return true;
}

void Model::render(glm::mat4 transform) {
    for(std::pair<std::string, std::pair<glm::mat4, Mesh>> p : meshes){
        RenderEngine::setModelMatrix(transform * p.second.first);
        RenderEngine::updateMatrices();
        if(RenderEngine::getCurrentShader()->hasUniform("Model")){
            glUniformMatrix4fv(RenderEngine::getCurrentShader()->getUniform("Model"), 1, GL_FALSE, &(transform * p.second.first)[0][0]);
        }
        p.second.second.render();
    }
}

Mesh& Model::getMesh(std::string name){
    return meshes.at(name).second;
}
