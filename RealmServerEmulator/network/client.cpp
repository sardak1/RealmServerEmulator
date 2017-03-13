// Copyright 2017 Sardak Software, All Rights Reserved

#include <iostream>
#include "network/client.h"
#include "protocol/encryption/packet_encryption.h"

namespace
{

    const size_t DEFAULT_BUFFER_SIZE = 128;

}  // namespace

network::Client::Client(network::Socket socket, sockaddr_in* address)
    : socket_(socket), receive_buffer_(DEFAULT_BUFFER_SIZE), send_buffer_(DEFAULT_BUFFER_SIZE)
{
    encryption_state_ = protocol::create_encryption_state();

    inet_ntop(AF_INET, &address->sin_addr, address_, sizeof(address_));
    std::cout << "Client connected from " << address_ << std::endl;
}

network::Client::~Client()
{
    disconnect();
}

void network::Client::receive_bytes()
{
    if (is_connected())
    {
        auto data = reinterpret_cast<char*>(receive_buffer_.data() + receive_buffer_.size());
        auto bytes_available = receive_buffer_.capacity() - receive_buffer_.size();
        auto bytes_read = ::recv(socket_, data, bytes_available, 0);

        if (bytes_read > 0)
        {
            receive_buffer_.advance(bytes_read);

            if (receive_buffer_.size() >= 4)
            {
                auto payload_size = *(reinterpret_cast<uint32_t*>(receive_buffer_.data()));
                receive_buffer_.grow(payload_size + 8);
            }
        }
        else
        {
            if (SOCKET_ERROR == bytes_read)
            {
                std::cerr << "Error while receiving packet from client at " << address_ << std::endl;
                log_socket_error();
            }

            disconnect();
        }
    }
}

void network::Client::send_bytes()
{
    if (is_connected())
    {
        auto bytes_sent = ::send(socket_, reinterpret_cast<const char*>(send_buffer_.data()), send_buffer_.size(), 0);

        if (SOCKET_ERROR != bytes_sent)
        {
            send_buffer_.consume(bytes_sent);
        }
        else
        {
            std::cerr << "Error while sending packet to client at " << address_ << std::endl;
            log_socket_error();
            disconnect();
        }
    }
}

void network::Client::send(protocol::NetMessage* message)
{
    auto offset = send_buffer_.size();
    send_buffer_.grow(offset + 8);

    auto data = reinterpret_cast<uint32_t*>(send_buffer_.data() + offset);
    data[1] = static_cast<uint32_t>(message->type());
    data[2] = server_id_;
    data[3] = 0;
    send_buffer_.advance(16);

    message->serialize(&send_buffer_);

    auto payload_size = send_buffer_.size() - offset - 4;
    payload_size += (4 - (payload_size & 3)) & 3;

    auto packet_end = offset + payload_size + 8;
    send_buffer_.grow(packet_end);
    send_buffer_.advance(packet_end - send_buffer_.size());

    data = reinterpret_cast<uint32_t*>(send_buffer_.data() + offset);
    data[0] = payload_size;

    protocol::encrypt_packet(&encryption_state_, data);
}

void network::Client::disconnect()
{
    if (is_connected())
    {
        std::cout << "Client disconnected from " << address_ << std::endl;

        ::closesocket(socket_);
        socket_ = INVALID_SOCKET;
    }
}
