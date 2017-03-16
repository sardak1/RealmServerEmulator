// Copyright 2017 Sardak Software, All Rights Reserved

#ifndef REALMSERVEREMULATOR_PROTOCOL_NET_MESSAGE_H_
#define REALMSERVEREMULATOR_PROTOCOL_NET_MESSAGE_H_

#include <string>
#include <vector>
#include "protocol/message_set.h"
#include "scripting/lua_include.h"
#include "network/buffer.h"
#include "serialize/json.h"

namespace protocol
{

    class NetMessage final
    {
    public:
        NetMessage() = delete;
        NetMessage(const NetMessage&) = delete;
        NetMessage(const MessageSet<NetMessage>& messages, nlohmann::json& data, const NetMessage* base);
        ~NetMessage() = default;

        void serialize(lua_State* lua, network::Buffer& buffer);
        void deserialize(lua_State* lua, network::Buffer& buffer);

        const std::string& name() const { return name_; }
        uint32_t type() const { return type_; }

    private:
        void copy_base(const NetMessage* base);
        void add_fields(nlohmann::json& fields);

        const MessageSet<NetMessage>& messages_;
        std::vector<nlohmann::json> fields_;
        std::string name_;
        uint32_t type_;
    };

}  // namespace protocol

#endif  // REALMSERVEREMULATOR_PROTOCOL_NET_MESSAGE_H_
