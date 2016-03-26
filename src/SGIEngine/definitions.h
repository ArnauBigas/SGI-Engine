/*
 * File:   definitions.h
 * Author: TheArni
 *
 * Created on 16 de febrero de 2015, 19:09
 */

#ifndef DEFINITIONS_H
#define	DEFINITIONS_H

#if __unix || __unix__ || __linux__
#define LINUX
#endif
#if _WIN32
#define WINDOWS
#endif

#define FGAMEDATA   "game_data/"

#define FRESOURCES  "resources/"
#define FSHRDTEXT   FRESOURCES"shared_textures/"
#define FSHADERS    FRESOURCES"shaders/"
#define FGUI	    FRESOURCES"gui/"
#define FMAPS       FRESOURCES"maps/"
#define FAUDIO	    FRESOURCES"audio/"

#define FEFFECTS    FAUDIO"effects/"
#define FMUSIC	    FAUDIO"music/"
#define FAMBIENT    FAUDIO"ambient/"

#define FPROPS	    "props/"
#define FWALLS      "walls/"

#define BUFFER_OFFSET(i) ((void*)(i))

#define GUITEXTUREUNIT              0
#define DIFFUSETEXTUREUNIT          1
#define NORMALMAPTEXTUREUNIT        2
#define SPECULARMAPTEXTUREUNIT      3
#define SHADOWMAPTEXTUREUNIT        4
#define DIFFUSEBUFFERTEXTUREUNIT    5
#define NORMALBUFFERTEXTUREUNIT     6
#define MATERIALBUFFERTEXTUREUNIT   7
#define DEPTHBUFFERTEXTUREUNIT      8
#define RANDOMNOISETEXTUREUNIT      9
#define SSAOTEXTUREUNIT             10

#define GUISHADER           "gui"
#define POINTLIGHTSHADER    "pointlight"
#define SPOTLIGHTSHADER     "spotlight"
#define AMBIENTSHADER       "ambient"
#define SHADOWMAPPINGSHADER "shadowmap"
#define SSAOSHADER          "ssao"

#endif	/* DEFINITIONS_H */

