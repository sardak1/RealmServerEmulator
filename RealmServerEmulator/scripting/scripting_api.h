// Copyright 2017 Sardak Software

#ifndef REALMSERVEREMULATOR_SCRIPTING_SCRIPTING_API_H_
#define REALMSERVEREMULATOR_SCRIPTING_SCRIPTING_API_H_

#include "scripting/lua_include.h"

namespace scripting
{
    lua_State* create_environment();

    void load_script(lua_State* lua, const char* filename);
    bool safe_call(lua_State* lua, int parameter_count, int return_count);
}

#endif  // REALMSERVEREMULATOR_SCRIPTING_SCRIPTING_API_H_
