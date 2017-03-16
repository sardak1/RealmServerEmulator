// Copyright 2017 Sardak Software, All Rights Reserved

#include "scripting/lua_server.h"
#include "network/client.h"
#include "server_state.h"

int scripting::lua_server_send_message(lua_State* lua)
{
    auto top = lua_gettop(lua);
    auto state = reinterpret_cast<ServerState*>(lua_touserdata(lua, lua_upvalueindex(1)));
    auto client = reinterpret_cast<network::Client*>(lua_touserdata(lua, 1));
    std::string message_name = luaL_checkstring(lua, 2);

    if (top > 2)
    {
        lua_pushvalue(lua, 3);
    }
    else
    {
        lua_newtable(lua);
    }

    auto message = state->protocol->get_message(message_name);
    if (message != nullptr)
    {
        client->send(message, lua);
    }

    lua_pop(lua, 1);

    return 0;
}
