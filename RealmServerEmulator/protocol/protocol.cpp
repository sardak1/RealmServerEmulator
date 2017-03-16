// Copyright 2017 Sardak Software, All Rights Reserved

#include "protocol/protocol.h"

protocol::Protocol::Protocol(nlohmann::json& packet_types, nlohmann::json& protocol)
{
    load_packet_types(packet_types);
    load_messages(protocol["messages"]);
}

protocol::NetMessage* protocol::Protocol::get_message(const std::string& name)
{
    return messages_.get_message(name);
}

void protocol::Protocol::load_packet_types(nlohmann::json& packet_types)
{
    for (nlohmann::json::iterator iter = packet_types.begin(); iter != packet_types.end(); iter++)
    {
        messages_.add_packet_type(iter.key(), iter.value().get<uint32_t>());
    }
}

void protocol::Protocol::load_messages(nlohmann::json& messages)
{
    for (auto& message : messages)
    {
        const NetMessage* base_message = nullptr;
        auto& iter = message.find("base");

        if (iter != message.end())
        {
            base_message = messages_.get_message(iter.value().get<std::string>());
        }

        std::shared_ptr<NetMessage> net_message(new NetMessage(messages_, message, base_message));
        messages_.add_message(net_message->name(), net_message);
    }
}
