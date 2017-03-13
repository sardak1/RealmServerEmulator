// Copyright 2017 Sardak Software

#include <iostream>
#include "scripting/lua_log.h"

int scripting::lua_log_info(lua_State* lua)
{
    std::cout << "Lua: " << luaL_checkstring(lua, -1) << std::endl;
    return 0;
}

int scripting::lua_log_error(lua_State* lua)
{
    std::cerr << "Lua: Error: " << luaL_checkstring(lua, -1) << std::endl;
    return 0;
}
