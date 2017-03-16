// Copyright 2017 Sardak Software, All Rights Reserved

#include <unordered_map>
#include "protocol/net_message_serializer.h"

namespace
{

    template<typename T, typename std::enable_if<std::is_integral<T>::value>::type* = nullptr>
    class IntegralSerializer final : public protocol::NetMessageSerializer
    {
    public:
        void serialize(const protocol::MessageSet<protocol::NetMessage>& message_set, nlohmann::json& definition, lua_State* lua, network::Buffer& buffer)
        {
            T value;

            if (lua_isnumber(lua, -1))
            {
                value = static_cast<T>(lua_tonumber(lua, -1));
            }
            else
            {
                auto& iter = definition.find("default");
                if (iter != definition.end())
                {
                    value = iter.value().get<T>();
                }
            }

            buffer.write<T>(value);
        }

        void deserialize(const protocol::MessageSet<protocol::NetMessage>& message_set, nlohmann::json& definition, lua_State* lua, network::Buffer& buffer)
        {
            lua_pushnumber(lua, buffer.read<T>());
        }
    };

    class PacketTypeSerializer final : public protocol::NetMessageSerializer
    {
        void serialize(const protocol::MessageSet<protocol::NetMessage>& message_set, nlohmann::json& definition, lua_State* lua, network::Buffer& buffer)
        {
            uint32_t value;

            if (lua_isstring(lua, -1))
            {
                value = message_set.get_packet_type(std::string(lua_tostring(lua, -1)));
            }
            else
            {
                auto& iter = definition.find("default");
                if (iter != definition.end())
                {
                    value = message_set.get_packet_type(iter.value().get<std::string>());
                }
            }

            buffer.write<uint32_t>(value);
        }

        void deserialize(const protocol::MessageSet<protocol::NetMessage>& message_set, nlohmann::json& definition, lua_State* lua, network::Buffer& buffer)
        {
            lua_pushstring(lua, message_set.get_packet_name(buffer.read<uint32_t>()).c_str());
        }
    };

    std::unordered_map<std::string, std::shared_ptr<protocol::NetMessageSerializer>> serializers = {
        { "int8", std::shared_ptr<protocol::NetMessageSerializer>(new IntegralSerializer<int8_t>()) },
        { "uint8", std::shared_ptr<protocol::NetMessageSerializer>(new IntegralSerializer<uint8_t>()) },
        { "int16", std::shared_ptr<protocol::NetMessageSerializer>(new IntegralSerializer<int16_t>()) },
        { "uint16", std::shared_ptr<protocol::NetMessageSerializer>(new IntegralSerializer<uint16_t>()) },
        { "int32", std::shared_ptr<protocol::NetMessageSerializer>(new IntegralSerializer<int32_t>()) },
        { "uint32", std::shared_ptr<protocol::NetMessageSerializer>(new IntegralSerializer<uint32_t>()) },
        { "packet_type", std::shared_ptr<protocol::NetMessageSerializer>(new PacketTypeSerializer()) },
    };

}  // namespace


protocol::NetMessageSerializer* protocol::NetMessageSerializer::get(const std::string& name)
{
    return serializers.find(name)->second.get();
}
