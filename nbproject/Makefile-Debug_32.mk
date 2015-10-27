#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=MinGW-Windows
CND_DLIB_EXT=dll
CND_CONF=Debug_32
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/Timer.o \
	${OBJECTDIR}/src/SGIEngine/ArgumentHandler.o \
	${OBJECTDIR}/src/SGIEngine/AudioData.o \
	${OBJECTDIR}/src/SGIEngine/AudioEngine.o \
	${OBJECTDIR}/src/SGIEngine/AudioObject.o \
	${OBJECTDIR}/src/SGIEngine/Button.o \
	${OBJECTDIR}/src/SGIEngine/Camera.o \
	${OBJECTDIR}/src/SGIEngine/Config.o \
	${OBJECTDIR}/src/SGIEngine/ControllableEntity.o \
	${OBJECTDIR}/src/SGIEngine/Entity.o \
	${OBJECTDIR}/src/SGIEngine/FirstPersonCamera.o \
	${OBJECTDIR}/src/SGIEngine/Game.o \
	${OBJECTDIR}/src/SGIEngine/Gui.o \
	${OBJECTDIR}/src/SGIEngine/GuiElement.o \
	${OBJECTDIR}/src/SGIEngine/GuiState.o \
	${OBJECTDIR}/src/SGIEngine/Label.o \
	${OBJECTDIR}/src/SGIEngine/Light.o \
	${OBJECTDIR}/src/SGIEngine/LightEmitterModule.o \
	${OBJECTDIR}/src/SGIEngine/LoadingState.o \
	${OBJECTDIR}/src/SGIEngine/LogicEngine.o \
	${OBJECTDIR}/src/SGIEngine/Material.o \
	${OBJECTDIR}/src/SGIEngine/Mesh.o \
	${OBJECTDIR}/src/SGIEngine/Model.o \
	${OBJECTDIR}/src/SGIEngine/ObjectModule.o \
	${OBJECTDIR}/src/SGIEngine/PositionalAudioObject.o \
	${OBJECTDIR}/src/SGIEngine/Prop.o \
	${OBJECTDIR}/src/SGIEngine/RenderEngine.o \
	${OBJECTDIR}/src/SGIEngine/ResourceEngine.o \
	${OBJECTDIR}/src/SGIEngine/Shader.o \
	${OBJECTDIR}/src/SGIEngine/SoundEmitterModule.o \
	${OBJECTDIR}/src/SGIEngine/SphereCollider.o \
	${OBJECTDIR}/src/SGIEngine/TaskEngine.o \
	${OBJECTDIR}/src/SGIEngine/Terrain.o \
	${OBJECTDIR}/src/SGIEngine/Texture.o \
	${OBJECTDIR}/src/SGIEngine/TexturedGuiQuad.o \
	${OBJECTDIR}/src/SGIEngine/Utility.o \
	${OBJECTDIR}/src/SGIEngine/Wall.o \
	${OBJECTDIR}/src/SGIEngine/World.o \
	${OBJECTDIR}/src/SGIEngine/WorldObject.o \
	${OBJECTDIR}/src/SGIEngine/WorldState.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-m32 -std=c++11 -ggdb
CXXFLAGS=-m32 -std=c++11 -ggdb

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsgiengine.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsgiengine.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsgiengine.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsgiengine.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsgiengine.a

${OBJECTDIR}/Timer.o: Timer.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Timer.o Timer.cpp

${OBJECTDIR}/src/SGIEngine/ArgumentHandler.o: src/SGIEngine/ArgumentHandler.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/SGIEngine
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/SGIEngine/ArgumentHandler.o src/SGIEngine/ArgumentHandler.cpp

${OBJECTDIR}/src/SGIEngine/AudioData.o: src/SGIEngine/AudioData.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/SGIEngine
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/SGIEngine/AudioData.o src/SGIEngine/AudioData.cpp

${OBJECTDIR}/src/SGIEngine/AudioEngine.o: src/SGIEngine/AudioEngine.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/SGIEngine
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/SGIEngine/AudioEngine.o src/SGIEngine/AudioEngine.cpp

${OBJECTDIR}/src/SGIEngine/AudioObject.o: src/SGIEngine/AudioObject.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/SGIEngine
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/SGIEngine/AudioObject.o src/SGIEngine/AudioObject.cpp

${OBJECTDIR}/src/SGIEngine/Button.o: src/SGIEngine/Button.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/SGIEngine
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/SGIEngine/Button.o src/SGIEngine/Button.cpp

${OBJECTDIR}/src/SGIEngine/Camera.o: src/SGIEngine/Camera.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/SGIEngine
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/SGIEngine/Camera.o src/SGIEngine/Camera.cpp

${OBJECTDIR}/src/SGIEngine/Config.o: src/SGIEngine/Config.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/SGIEngine
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/SGIEngine/Config.o src/SGIEngine/Config.cpp

${OBJECTDIR}/src/SGIEngine/ControllableEntity.o: src/SGIEngine/ControllableEntity.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/SGIEngine
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/SGIEngine/ControllableEntity.o src/SGIEngine/ControllableEntity.cpp

${OBJECTDIR}/src/SGIEngine/Entity.o: src/SGIEngine/Entity.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/SGIEngine
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/SGIEngine/Entity.o src/SGIEngine/Entity.cpp

${OBJECTDIR}/src/SGIEngine/FirstPersonCamera.o: src/SGIEngine/FirstPersonCamera.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/SGIEngine
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/SGIEngine/FirstPersonCamera.o src/SGIEngine/FirstPersonCamera.cpp

${OBJECTDIR}/src/SGIEngine/Game.o: src/SGIEngine/Game.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/SGIEngine
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/SGIEngine/Game.o src/SGIEngine/Game.cpp

${OBJECTDIR}/src/SGIEngine/Gui.o: src/SGIEngine/Gui.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/SGIEngine
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/SGIEngine/Gui.o src/SGIEngine/Gui.cpp

${OBJECTDIR}/src/SGIEngine/GuiElement.o: src/SGIEngine/GuiElement.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/SGIEngine
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/SGIEngine/GuiElement.o src/SGIEngine/GuiElement.cpp

${OBJECTDIR}/src/SGIEngine/GuiState.o: src/SGIEngine/GuiState.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/SGIEngine
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/SGIEngine/GuiState.o src/SGIEngine/GuiState.cpp

${OBJECTDIR}/src/SGIEngine/Label.o: src/SGIEngine/Label.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/SGIEngine
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/SGIEngine/Label.o src/SGIEngine/Label.cpp

${OBJECTDIR}/src/SGIEngine/Light.o: src/SGIEngine/Light.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/SGIEngine
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/SGIEngine/Light.o src/SGIEngine/Light.cpp

${OBJECTDIR}/src/SGIEngine/LightEmitterModule.o: src/SGIEngine/LightEmitterModule.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/SGIEngine
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/SGIEngine/LightEmitterModule.o src/SGIEngine/LightEmitterModule.cpp

${OBJECTDIR}/src/SGIEngine/LoadingState.o: src/SGIEngine/LoadingState.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/SGIEngine
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/SGIEngine/LoadingState.o src/SGIEngine/LoadingState.cpp

${OBJECTDIR}/src/SGIEngine/LogicEngine.o: src/SGIEngine/LogicEngine.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/SGIEngine
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/SGIEngine/LogicEngine.o src/SGIEngine/LogicEngine.cpp

${OBJECTDIR}/src/SGIEngine/Material.o: src/SGIEngine/Material.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/SGIEngine
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/SGIEngine/Material.o src/SGIEngine/Material.cpp

${OBJECTDIR}/src/SGIEngine/Mesh.o: src/SGIEngine/Mesh.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/SGIEngine
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/SGIEngine/Mesh.o src/SGIEngine/Mesh.cpp

${OBJECTDIR}/src/SGIEngine/Model.o: src/SGIEngine/Model.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/SGIEngine
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/SGIEngine/Model.o src/SGIEngine/Model.cpp

${OBJECTDIR}/src/SGIEngine/ObjectModule.o: src/SGIEngine/ObjectModule.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/SGIEngine
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/SGIEngine/ObjectModule.o src/SGIEngine/ObjectModule.cpp

${OBJECTDIR}/src/SGIEngine/PositionalAudioObject.o: src/SGIEngine/PositionalAudioObject.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/SGIEngine
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/SGIEngine/PositionalAudioObject.o src/SGIEngine/PositionalAudioObject.cpp

${OBJECTDIR}/src/SGIEngine/Prop.o: src/SGIEngine/Prop.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/SGIEngine
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/SGIEngine/Prop.o src/SGIEngine/Prop.cpp

${OBJECTDIR}/src/SGIEngine/RenderEngine.o: src/SGIEngine/RenderEngine.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/SGIEngine
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/SGIEngine/RenderEngine.o src/SGIEngine/RenderEngine.cpp

${OBJECTDIR}/src/SGIEngine/ResourceEngine.o: src/SGIEngine/ResourceEngine.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/SGIEngine
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/SGIEngine/ResourceEngine.o src/SGIEngine/ResourceEngine.cpp

${OBJECTDIR}/src/SGIEngine/Shader.o: src/SGIEngine/Shader.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/SGIEngine
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/SGIEngine/Shader.o src/SGIEngine/Shader.cpp

${OBJECTDIR}/src/SGIEngine/SoundEmitterModule.o: src/SGIEngine/SoundEmitterModule.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/SGIEngine
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/SGIEngine/SoundEmitterModule.o src/SGIEngine/SoundEmitterModule.cpp

${OBJECTDIR}/src/SGIEngine/SphereCollider.o: src/SGIEngine/SphereCollider.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/SGIEngine
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/SGIEngine/SphereCollider.o src/SGIEngine/SphereCollider.cpp

${OBJECTDIR}/src/SGIEngine/TaskEngine.o: src/SGIEngine/TaskEngine.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/SGIEngine
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/SGIEngine/TaskEngine.o src/SGIEngine/TaskEngine.cpp

${OBJECTDIR}/src/SGIEngine/Terrain.o: src/SGIEngine/Terrain.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/SGIEngine
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/SGIEngine/Terrain.o src/SGIEngine/Terrain.cpp

${OBJECTDIR}/src/SGIEngine/Texture.o: src/SGIEngine/Texture.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/SGIEngine
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/SGIEngine/Texture.o src/SGIEngine/Texture.cpp

${OBJECTDIR}/src/SGIEngine/TexturedGuiQuad.o: src/SGIEngine/TexturedGuiQuad.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/SGIEngine
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/SGIEngine/TexturedGuiQuad.o src/SGIEngine/TexturedGuiQuad.cpp

${OBJECTDIR}/src/SGIEngine/Utility.o: src/SGIEngine/Utility.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/SGIEngine
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/SGIEngine/Utility.o src/SGIEngine/Utility.cpp

${OBJECTDIR}/src/SGIEngine/Wall.o: src/SGIEngine/Wall.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/SGIEngine
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/SGIEngine/Wall.o src/SGIEngine/Wall.cpp

${OBJECTDIR}/src/SGIEngine/World.o: src/SGIEngine/World.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/SGIEngine
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/SGIEngine/World.o src/SGIEngine/World.cpp

${OBJECTDIR}/src/SGIEngine/WorldObject.o: src/SGIEngine/WorldObject.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/SGIEngine
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/SGIEngine/WorldObject.o src/SGIEngine/WorldObject.cpp

${OBJECTDIR}/src/SGIEngine/WorldState.o: src/SGIEngine/WorldState.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/SGIEngine
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/SGIEngine/WorldState.o src/SGIEngine/WorldState.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsgiengine.a

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
