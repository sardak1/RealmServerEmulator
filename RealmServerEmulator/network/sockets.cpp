// Copyright 2017 Sardak Software, All Rights Reserved

#include <iostream>
#include "network/sockets.h"

void network::init_sockets()
{
#ifdef WIN32
    WSADATA winsockData;
    auto winsockVersion = MAKEWORD(2, 2);

    WSAStartup(winsockVersion, &winsockData);
#endif  // WIN32
}

void network::shutdown_sockets()
{
#ifdef WIN32
    WSACleanup();
#endif  // WIN32
}

void network::log_socket_error()
{
#ifdef WIN32
    char* buffer = nullptr;

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        WSAGetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        reinterpret_cast<LPSTR>(&buffer),
        0,
        nullptr
    );

    if (nullptr != buffer)
    {
        std::cerr << "Winsock error: " << buffer << std::endl;
        LocalFree(buffer);
    }
#endif  // WIN32
}
