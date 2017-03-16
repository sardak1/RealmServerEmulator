// Copyright 2017 Sardak Software, All Rights Reserved

#ifndef REALMSERVEREMULATOR_SCRIPTING_SCRIPTING_API_H_
#define REALMSERVEREMULATOR_SCRIPTING_SCRIPTING_API_H_

#include "scripting/lua_include.h"
#include "server_state.h"

namespace scripting
{
    lua_State* create_environment(ServerState* state);

    void load_script(lua_State* lua, const char* filename);
    bool safe_call(lua_State* lua, int parameter_count, int return_count);
}

#endif  // REALMSERVEREMULATOR_SCRIPTING_SCRIPTING_API_H_
