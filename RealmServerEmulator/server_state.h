// Copyright 2017 Sardak Software, All Rights Reserved

#ifndef REALMSERVEREMULATOR_SERVER_STATE_H_
#define REALMSERVEREMULATOR_SERVER_STATE_H_

#include <vector>
#include <memory>
#include "network/client.h"
#include "scripting/lua_include.h"
#include "protocol/protocol.h"

struct ServerState
{
    std::shared_ptr<protocol::Protocol> protocol;
    std::vector<std::shared_ptr<network::Client>> clients;
    lua_State* lua;
};

#endif  // REALMSERVEREMULATOR_SERVER_STATE_H_
