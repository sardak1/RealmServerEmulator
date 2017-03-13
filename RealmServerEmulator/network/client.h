// Copyright 2017 Sardak Software

#ifndef REALMSERVEREMULATOR_NETWORK_CLIENT_H_
#define REALMSERVEREMULATOR_NETWORK_CLIENT_H_

#include <cstdint>
#include "network/sockets.h"
#include "network/buffer.h"
#include "protocol/encryption/encryption_state.h"
#include "protocol/net_message.h"

namespace network
{

    class Client final
    {
    public:
        Client() = delete;
        Client(Socket socket, sockaddr_in* address);
        Client(const Client&) = delete;
        ~Client();

        Client& operator=(const Client&) = delete;

        void receive_bytes();
        void send_bytes();

        void send(protocol::NetMessage* message);
        void disconnect();

        bool is_connected() const { return INVALID_SOCKET != socket_; }

        Socket socket() const { return socket_; }
        protocol::EncryptionState* encryption_state() { return &encryption_state_; }
        Buffer* receive_buffer() { return &receive_buffer_; }
        Buffer* send_buffer() { return &send_buffer_; }
        uint32_t server_id() const { return server_id_; }

    private:
        Socket socket_;
        protocol::EncryptionState encryption_state_;
        char address_[32];
        Buffer receive_buffer_;
        Buffer send_buffer_;
        uint32_t server_id_ = 0;
    };

}  // namespace network

#endif  // REALMSERVEREMULATOR_NETWORK_CLIENT_H_

