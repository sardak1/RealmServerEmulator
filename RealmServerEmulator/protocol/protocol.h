// Copyright 2017 Sardak Software, All Rights Reserved

#ifndef REALMSERVEREMULATOR_PROTOCOL_PROTOCOL_H_
#define REALMSERVEREMULATOR_PROTOCOL_PROTOCOL_H_

#include <unordered_map>
#include <string>
#include "protocol/net_message.h"
#include "protocol/message_set.h"
#include "serialize/json.h"

namespace protocol
{

    class Protocol final
    {
    public:
        Protocol() = delete;
        Protocol(const Protocol&) = delete;
        Protocol(nlohmann::json& packet_types, nlohmann::json& protocol);

        NetMessage* get_message(const std::string& name);

        Protocol& operator=(const Protocol&) = delete;

    private:
        void load_packet_types(nlohmann::json& packet_types);
        void load_messages(nlohmann::json& messages);

        MessageSet<NetMessage> messages_;
    };

}  // namespace protocol

#endif  // REALMSERVEREMULATOR_PROTOCOL_PROTOCOL_H_
