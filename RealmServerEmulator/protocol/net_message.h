// Copyright 2017 Sardak Software, All Rights Reserved

#ifndef REALMSERVEREMULATOR_PROTOCOL_NET_MESSAGE_H_
#define REALMSERVEREMULATOR_PROTOCOL_NET_MESSAGE_H_

#include "protocol/packet_type.h"
#include "network/buffer.h"

namespace protocol
{

    class NetMessage
    {
    public:
        virtual ~NetMessage() { }

        virtual PacketType type() const = 0;

        virtual void serialize(network::Buffer* buffer) = 0;
        virtual void deserialize(network::Buffer* buffer) = 0;
    };

    class ServerMessage : public NetMessage
    {
    public:
        void deserialize(network::Buffer*) override
        {
            // Not implemented
        }
    };

    class ClientMessage : public NetMessage
    {
    public:
        void serialize(network::Buffer*) override
        {
            // Not implemented
        }
    };

}

#endif  // REALMSERVEREMULATOR_PROTOCOL_NET_MESSAGE_H_
