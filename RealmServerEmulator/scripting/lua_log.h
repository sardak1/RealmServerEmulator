// Copyright 2017 Sardak Software, All Rights Reserved

#ifndef REALMSERVEREMULATOR_SCRIPTING_LUA_LOG_H_
#define REALMSERVEREMULATOR_SCRIPTING_LUA_LOG_H_

#include "scripting/lua_include.h"

namespace scripting
{

    int lua_log_info(lua_State* lua);
    int lua_log_error(lua_State* lua);

}  // namespace scripting

#endif  // REALMSERVEREMULATOR_SCRIPTING_LUA_LOG_H_
