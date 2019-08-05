#ifndef SERVER_HPP
#define SERVER_HPP

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <thread>
#include <mutex>

class Server
{
public:
    Server();

    int initialize_socket();
    int listen();
    int accept();
private:
    WSADATA wsaData;
    std::string default_port = "27015";
    SOCKET ListenSocket;
    std::vector<SOCKET> Clients;
    unsigned last_client_id;
    std::mutex mtx;
};

#endif // SERVER_HPP
