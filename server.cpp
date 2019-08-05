#include "server.hpp"

#pragma comment(lib, "Ws2_32.lib")

Server::Server()
{
    int iResult;
    iResult = WSAStartup(MAKEWORD(2, 2), &this->wsaData);
    if (iResult != 0)
    {
        printf("WSAStartup failed: %d\n", iResult);
        exit(iResult);
    }

    this->ListenSocket = INVALID_SOCKET;

    this->Clients.resize(20);
    this->last_client_id = 0;
}

int Server::initialize_socket()
{
    int iResult;

    struct addrinfo *result = nullptr, *ptr = nullptr, hints;

    ZeroMemory(&hints, sizeof (hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    iResult = getaddrinfo(0, this->default_port.c_str(), &hints, &result);

    if (iResult != 0)
    {
        printf("getaddrinfo failed: %d\n", iResult);
        WSACleanup();
        exit(iResult);
    }

    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    if (ListenSocket == INVALID_SOCKET)
    {
        printf("Error at socket(): %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        exit(-1);
    }

    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);

    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        exit(iResult);
    }

    freeaddrinfo(result);

    return 0;
}

int Server::listen()
{
    if (::listen(ListenSocket, SOMAXCONN ) == SOCKET_ERROR)
    {
        printf("Listen failed with error: %ld\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        exit(SOCKET_ERROR);
    }

    return 0;
}

int Server::accept()
{
    while (this->last_client_id < Clients.size())
    {
        SOCKET new_client = INVALID_SOCKET;
        new_client = ::accept(ListenSocket, 0, 0);
        if (new_client == INVALID_SOCKET)
        {
            printf("accept failed: %d\n", WSAGetLastError());
            closesocket(new_client);
            WSACleanup();
        }
        else
        {
            mtx.lock();
            this->Clients[this->last_client_id++] = new_client;
            mtx.unlock();
        }
    }
}
























