// Copyright 2017 Sardak Software, All Rights Reserved

#include "protocol/net_message.h"
#include "protocol/net_message_serializer.h"

protocol::NetMessage::NetMessage(const MessageSet<NetMessage>& messages, nlohmann::json& data, const protocol::NetMessage* base)
    : messages_(messages)
{
    copy_base(base);

    name_ = data["name"].get<std::string>();

    auto iter = data.find("type");

    if (iter != data.end())
    {
        type_ = messages.get_packet_type(iter.value().get<std::string>());
    }

    iter = data.find("fields");

    if (iter != data.end())
    {
        add_fields(iter.value());
    }
}

void protocol::NetMessage::serialize(lua_State* lua, network::Buffer& buffer)
{
    NetMessage* message;

    for (auto& field : fields_)
    {
        const std::string& name = field["name"];
        const std::string& type = field["type"];
        lua_getfield(lua, -1, name.c_str());

        message = messages_.get_message(type);

        if (message != nullptr)
        {
            message->serialize(lua, buffer);
        }
        else
        {
            NetMessageSerializer::get(type)->serialize(messages_, field, lua, buffer);
        }

        lua_pop(lua, 1);
    }
}

void protocol::NetMessage::deserialize(lua_State* lua, network::Buffer& buffer)
{
    NetMessage* message;

    lua_newtable(lua);

    for (auto& field : fields_)
    {
        const std::string& name = field["name"];
        message = messages_.get_message(name);

        if (message != nullptr)
        {
            message->deserialize(lua, buffer);
        }
        else
        {
            NetMessageSerializer::get(name)->deserialize(messages_, field, lua, buffer);
        }

        lua_setfield(lua, -2, name.c_str());
    }
}

void protocol::NetMessage::copy_base(const protocol::NetMessage* base)
{
    if (base != nullptr)
    {
        for (auto& field : base->fields_)
        {
            fields_.push_back(field);
        }

        type_ = base->type();
    }
}

void protocol::NetMessage::add_fields(nlohmann::json& fields)
{
    std::unordered_map<std::string, nlohmann::json*> existing_fields;

    for (nlohmann::json& field : fields_)
    {
        existing_fields.insert(std::make_pair(field["name"].get<std::string>(), &field));
    }

    for (auto& field : fields)
    {
        auto iter = existing_fields.find(field["name"].get<std::string>());

        if (iter != existing_fields.end())
        {
            for (nlohmann::json::iterator pair = field.begin(); pair != field.end(); pair++)
            {
                (*(iter->second))[pair.key()] = pair.value();
            }
        }
        else
        {
            fields_.push_back(field);
        }
    }
}
