// Copyright 2017 Sardak Software, All Rights Reserved

#ifndef REALMSERVEREMULATOR_PROTOCOL_NET_MESSAGE_SERIALIZER_H_
#define REALMSERVEREMULATOR_PROTOCOL_NET_MESSAGE_SERIALIZER_H_

#include <string>
#include "network/buffer.h"
#include "scripting/lua_include.h"
#include "protocol/message_set.h"
#include "protocol/net_message.h"
#include "serialize/json.h"

namespace protocol
{

    class NetMessageSerializer
    {
    public:
        static NetMessageSerializer* get(const std::string& name);

        virtual void serialize(const MessageSet<NetMessage>& message_set, nlohmann::json& definition, lua_State* lua, network::Buffer& buffer) = 0;
        virtual void deserialize(const MessageSet<NetMessage>& message_set, nlohmann::json& definition, lua_State* lua, network::Buffer& buffer) = 0;
    };

}  // namespace protocol

#endif  // REALMSERVEREMULATOR_PROTOCOL_NET_MESSAGE_SERIALIZER_H_
