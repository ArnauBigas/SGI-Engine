/* 
 * File:   LogicEngine.cpp
 * Author: arnau
 * 
 * Created on 12 / d’agost / 2015, 18:52
 */

#include "LogicEngine.h"

/*extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}*/

//#include <iostream>
#include "ObjectModule.h"
#include "SoundEmitterModule.h"
#include "LightEmitterModule.h"

bool LogicEngine::init() {
    /*lua_State* state = luaL_newstate();
    luaL_openlibs(state);
    if (luaL_loadfile(state, "test.lua") != 0) {
        Logger::info << "couldn't load test file" << std::endl;
    }
    lua_call(state, 0, 0);
    lua_close(state);*/
    addObjectModule(new SoundEmitterModule());
    addObjectModule(new LightEmitterModule());
    return true;
}
