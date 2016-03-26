/* 
 * File:   ResourceEngine.h
 * Author: arnau
 *
 * Created on 11 / d’agost / 2015, 23:56
 */

#ifndef RESOURCEENGINE_H
#define	RESOURCEENGINE_H

#include <string>

std::string getLoadingStatus();

bool loaded();

void loadAllResources();

void loadAllDomainResources();

void loadProps(std::string domain);

void loadWalls(std::string domain);

void loadShaders();

void loadTextures();

void loadAudio();

void setResourceLocation(std::string location);

#endif	/* RESOURCEENGINE_H */

