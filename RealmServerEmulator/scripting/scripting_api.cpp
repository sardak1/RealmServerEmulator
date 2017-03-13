// Copyright 2017 Sardak Software, All Rights Reserved

#include <string>
#include <iostream>
#include "scripting/scripting_api.h"
#include "scripting/lua_log.h"

namespace
{
    void log_error(lua_State* lua)
    {
        auto error_message = lua_tostring(lua, -1);
        lua_pop(lua, 1);

        std::cerr << "Lua error: " << error_message << std::endl;
    }

    void get_debug_traceback(lua_State* lua)
    {
        lua_getglobal(lua, "debug");
        lua_getfield(lua, -1, "traceback");
        lua_remove(lua, -2);
    }

    void add_search_path(lua_State* lua, const char* path)
    {
        lua_getglobal(lua, "package");
        lua_getfield(lua, -1, "path");

        std::string package_path = lua_tostring(lua, -1);
        package_path.append(";");
        package_path.append(path);

        lua_pop(lua, 1);

        lua_pushstring(lua, package_path.c_str());
        lua_setfield(lua, -2, "path");
        lua_pop(lua, 1);
    }

    void register_log_functions(lua_State* lua)
    {
        lua_newtable(lua);

        lua_pushstring(lua, "info");
        lua_pushcclosure(lua, &scripting::lua_log_info, 0);
        lua_settable(lua, -3);

        lua_pushstring(lua, "error");
        lua_pushcclosure(lua, &scripting::lua_log_error, 0);
        lua_settable(lua, -3);

        lua_setglobal(lua, "Log");
    }

}  // namespace

lua_State* scripting::create_environment()
{
    auto lua = luaL_newstate();
    luaL_openlibs(lua);

    add_search_path(lua, "scripts/?.lua");
    register_log_functions(lua);

    return lua;
}

void scripting::load_script(lua_State* lua, const char* filename)
{
    std::cout << "Loading script: " << filename << std::endl;

    get_debug_traceback(lua);
    auto error_handler = lua_gettop(lua);

    if ((0 != luaL_loadfile(lua, filename)) || (0 != lua_pcall(lua, 0, 0, error_handler)))
    {
        log_error(lua);
    }

    lua_pop(lua, 1);
}

bool scripting::safe_call(lua_State* lua, int parameter_count, int return_count)
{
    auto success = true;

    get_debug_traceback(lua);
    lua_insert(lua, -(parameter_count + 2));

    if (0 != lua_pcall(lua, parameter_count, return_count, -(parameter_count + 2)))
    {
        log_error(lua);
        success = false;
    }

    lua_pop(lua, 1);

    return success;
}
