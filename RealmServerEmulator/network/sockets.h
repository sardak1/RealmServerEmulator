// Copyright 2017 Sardak Software

#ifndef REALMSERVEREMULATOR_NETWORK_SOCKETS_H_
#define REALMSERVEREMULATOR_NETWORK_SOCKETS_H_

#ifdef WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>
#endif  // WIN32

namespace network
{
    void init_sockets();
    void shutdown_sockets();

    void log_socket_error();

#ifdef WIN32
    typedef SOCKET Socket;
#endif
}

#endif  // REALMSERVEREMULATOR_NETWORK_SOCKETS_H_
