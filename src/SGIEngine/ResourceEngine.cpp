/* 
 * File:   ResourceEngine.cpp
 * Author: arnau
 * 
 * Created on 11 / d’agost / 2015, 23:56
 */

#include "ResourceEngine.h"

#include <vector>
#include <SDL2/SDL_audio.h>

#include "Utility.h"
#include "Prop.h"
#include "definitions.h"
#include "Shader.h"
#include "Texture.h"
#include "AudioData.h"
#include "Wall.h"
#include "Logger.h"

std::string loadingStatus;
std::string resourceLocation = "";

bool isLoaded = false;

std::string getLoadingStatus() {
    return loadingStatus;
}

void updateStatus(std::string status) {
    loadingStatus = status;
    Logger::info << status << std::endl;
}

bool loaded() {
    return isLoaded;
}

void loadAllResources() {
    loadTextures();
    loadShaders();
    loadAudio();
    loadAllDomainResources();
    isLoaded = true;
    updateStatus("All resources loaded");
}

void loadAllDomainResources() {
    std::vector<std::string> domains = getSubDirectories(resourceLocation + FGAMEDATA);
    for (std::string s : domains) {
        updateStatus("Loading domain " + s);
        loadProps(s);
        loadWalls(s);
    }
}

void loadProps(std::string domain) {
    std::vector<std::string> props = getSubDirectories(resourceLocation + FGAMEDATA + domain + "/" + FPROPS);
    for (std::string s : props) {
        updateStatus("Loading prop " + domain + ":" + s);
        registerWorldObject("prop:" + domain + ":" + s, new Prop(resourceLocation + FGAMEDATA + domain + "/" + FPROPS + s + "/"));
    }
}

void loadWalls(std::string domain) {
    std::vector<std::string> props = getSubDirectories(resourceLocation + FGAMEDATA + domain + "/" + FWALLS);
    for (std::string s : props) {
        updateStatus("Loading wall " + domain + ":" + s);
        registerWorldObject("wall:" + domain + ":" + s, new Wall(resourceLocation + FGAMEDATA + domain + "/" + FWALLS + s + "/"));
    }
}

void loadShaders() {
    updateStatus("Loading shaders...");
    std::vector<std::string> programs = getFilesList(resourceLocation + FSHADERS, "(\\.json)$");
    for (std::string program : programs) {
        updateStatus("Loading shader program " + program);
        addShader(remove_extension(program), new ShaderProgram(resourceLocation + FSHADERS, program));
    }
}

void loadTextures() {
    updateStatus("Loading shared textures...");
    std::vector<std::string> textures = getFilesList(resourceLocation + FSHRDTEXT, "(\\.png)$");
    for (std::string texture : textures) {
        updateStatus("Loading shared texture " + texture);
        addTexture(remove_extension(texture), loadTextureFromPNG(resourceLocation + FSHRDTEXT + texture));
    }
    textures.clear();
    updateStatus("Loading gui textures...");
    textures = getFilesList(resourceLocation + FGUI, "(\\.png)$");
    for (std::string texture : textures) {
        updateStatus("Loading gui texture " + texture);
        addTexture(remove_extension("gui:" + texture), loadTextureFromPNG(resourceLocation + FGUI + texture));
    }
}

void loadAudio(){
    updateStatus("Loading music files...");
    std::vector<std::string> musicFiles = getFilesList(resourceLocation + FMUSIC, "(\\.wav)$");
    for(std::string s : musicFiles){
        updateStatus("Loading music audio file " + s);
        AudioData* audio = loadAudioFromWAV(resourceLocation + FMUSIC + s);
        if(audio != 0){
            addAudioData(remove_extension(s), audio);
        }
    }
}

void setResourceLocation(std::string location){
    resourceLocation = location;
}