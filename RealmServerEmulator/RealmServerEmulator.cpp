// Copyright 2017 Sardak Software, All Rights Reserved

#include <vector>
#include <fstream>
#include <iostream>
#include <memory>
#include <thread>
#include "network/sockets.h"
#include "network/client.h"
#include "serialize/json.h"
#include "protocol/encryption/encryption_algorithm.h"
#include "protocol/encryption/packet_encryption.h"
#include "protocol/packet_util.h"
#include "protocol/ack.h"
#include "scripting/scripting_api.h"

#ifdef WIN32
#include <conio.h>
#endif

nlohmann::json load_config(const char* filename);
network::Socket start_listening(uint16_t port);
void accept_connection(network::Socket listen_socket, std::vector<std::shared_ptr<network::Client>>& clients);
void update_clients(std::vector<std::shared_ptr<network::Client>>& clients);
void handle_packet(std::shared_ptr<network::Client> client);
void disable_tcp_delay(network::Socket socket);

int main()
{
    auto config = load_config("config.json");
    std::vector<std::shared_ptr<network::Client>> clients;

    std::cout << "Starting server " << config["name"].get<std::string>() << std::endl;

    network::init_sockets();

    std::cout << "Initializing scripting environment" << std::endl;
    auto lua = scripting::create_environment();
    scripting::load_script(lua, "scripts/realm.lua");

    auto listen_socket = start_listening(config["port"].get<uint16_t>());

    if (INVALID_SOCKET != listen_socket)
    {
        while (true)
        {
#ifdef WIN32
            if (_kbhit())
            {
                auto key = _getch();

                if (VK_ESCAPE == key)
                {
                    break;
                }
            }
#endif

            accept_connection(listen_socket, clients);
            update_clients(clients);

            std::this_thread::yield();
        }

        clients.clear();

        std::cout << "Closing server socket" << std::endl;
        ::closesocket(listen_socket);

        std::cout << "Closing scripting environment" << std::endl;
        lua_close(lua);
    }

    network::shutdown_sockets();

    return 0;
}

nlohmann::json load_config(const char* filename)
{
    std::ifstream config_file;
    nlohmann::json config;

    config_file.open(filename, std::ifstream::in);

    if (config_file.is_open())
    {
        config << config_file;
        config_file.close();
    }
    else
    {
        std::cerr << "Could not open config file at: " << filename << std::endl;
    }

    return config;
}

network::Socket start_listening(uint16_t port)
{
    struct sockaddr_in address;

    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.S_un.S_addr = INADDR_ANY;

    auto listen_socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (INVALID_SOCKET == listen_socket)
    {
        std::cerr << "Failed to create server socket." << std::endl;
        return INVALID_SOCKET;
    }

    int result = ::bind(listen_socket, reinterpret_cast<struct sockaddr*>(&address), sizeof(sockaddr_in));

    if (SOCKET_ERROR == result)
    {
        ::closesocket(listen_socket);
        std::cerr << "Failed to bind server socket." << std::endl;
        return INVALID_SOCKET;
    }

    result = ::listen(listen_socket, SOMAXCONN);

    if (SOCKET_ERROR == result)
    {
        ::closesocket(listen_socket);
        std::cerr << "Failed to listen on server socket." << std::endl;
        return INVALID_SOCKET;
    }

    disable_tcp_delay(listen_socket);

    std::cout << "Now listening on port " << port << std::endl;

    return listen_socket;
}

bool do_select(fd_set* read, fd_set* write, fd_set* except, int num_fd)
{
    static timeval timeout{ 0, 0 };

    return ::select(num_fd, read, write, except, &timeout) > 0;
}

void accept_connection(network::Socket listen_socket, std::vector<std::shared_ptr<network::Client>>& clients)
{
    static fd_set read;

    FD_ZERO(&read);
    FD_SET(listen_socket, &read);

    if (do_select(&read, nullptr, nullptr, static_cast<int>(listen_socket) + 1))
    {
        sockaddr_in address;
        int address_size = sizeof(address);
        auto client_socket = ::accept(listen_socket, reinterpret_cast<struct sockaddr*>(&address), &address_size);

        disable_tcp_delay(client_socket);

        std::shared_ptr<network::Client> client(new network::Client(client_socket, &address));

        protocol::write_decryption_algorithm(client->encryption_state(), client->send_buffer());
        protocol::write_encryption_algorithm(client->encryption_state(), client->send_buffer());

        protocol::Ack ack(protocol::PacketType::kConnect);
        client->send(&ack);

        clients.push_back(client);
    }
}

void update_clients(std::vector<std::shared_ptr<network::Client>>& clients)
{
    static fd_set read, write, except;

    FD_ZERO(&read);
    FD_ZERO(&write);
    FD_ZERO(&except);

    network::Socket socket;
    int num_fds = 0;

    for (auto client : clients)
    {
        if (client->is_connected())
        {
            socket = client->socket();

            FD_SET(socket, &read);
            FD_SET(socket, &except);

            if (client->send_buffer()->size() > 0)
            {
                FD_SET(socket, &write);
            }

            num_fds = max(num_fds, static_cast<int>(socket));
        }
    }

    if (do_select(&read, &write, &except, num_fds))
    {
        for (auto client : clients)
        {
            socket = client->socket();

            if (FD_ISSET(socket, &except))
            {
                client->disconnect();
                continue;
            }

            if (FD_ISSET(socket, &read))
            {
                client->receive_bytes();

                if (client->is_connected())
                {
                    handle_packet(client);
                }
            }

            if (FD_ISSET(socket, &write))
            {
                client->send_bytes();
            }
        }
    }

    clients.erase(std::remove_if(
        clients.begin(),
        clients.end(),
        [](std::shared_ptr<network::Client> client) { return !client->is_connected(); }
    ), clients.end());
}

void handle_packet(std::shared_ptr<network::Client> client)
{
    auto buffer = client->receive_buffer();

    if (buffer->size() >= 4)
    {
        auto payload_size = *(reinterpret_cast<uint32_t*>(buffer->data()));

        if (buffer->size() >= (payload_size + 8))
        {
            if (protocol::decrypt_packet(client->encryption_state(), buffer->data()))
            {
                // Do something with the packet

                buffer->consume(payload_size + 8);
            }
            else
            {
                client->disconnect();
            }
        }
    }
}

void disable_tcp_delay(network::Socket socket)
{
    BOOL option_value = TRUE;
    int value_size = sizeof(BOOL);
    ::setsockopt(socket, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<char*>(&option_value), value_size);
}
