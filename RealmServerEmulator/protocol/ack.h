// Copyright 2017 Sardak Software

#ifndef REALMSERVEREMULATOR_PROTOCOL_ACK_H_
#define REALMSERVEREMULATOR_PROTOCOL_ACK_H_

#include "protocol/net_message.h"

namespace protocol
{

    class Ack : public ServerMessage
    {
    public:
        Ack() = delete;
        Ack(PacketType ack_type) : Ack(ack_type, 0) { }
        Ack(PacketType ack_type, uint32_t info) : ack_type_(ack_type), info_(info) { }

        PacketType type() const override { return PacketType::kAck; }

        void serialize(network::Buffer* buffer)
        {
            buffer->grow(buffer->size() + 8);

            buffer->write(static_cast<uint32_t>(ack_type_));
            buffer->write(info_);
        }

    private:
        PacketType ack_type_;
        uint32_t info_;
    };

}  // namespace protocol

#endif  // REALMSERVEREMULATOR_PROTOCOL_ACK_H_
