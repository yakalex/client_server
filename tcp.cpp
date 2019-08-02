#include "tcp.h"

void tcp_client ()
{
    std::cout << "client"<< std::endl;
    std::string addr;
    std::cin >> addr;
    int Socket = socket(
        AF_INET,
        SOCK_STREAM,
        IPPROTO_TCP);
    struct sockaddr_in SockAddr;
    struct sockaddr_in SockAddr2;
    SockAddr.sin_family = AF_INET;
    SockAddr.sin_port = htons(12345);
    inet_pton(AF_INET,addr.c_str(), &SockAddr.sin_addr.s_addr);
    //SockAddr.sin_addr.s_addr = inet_addr(addr.c_str());

    connect (Socket, reinterpret_cast<struct sockaddr *> (&SockAddr), sizeof(SockAddr));
    unsigned int size;
    recv(Socket, &size, sizeof(unsigned int),MSG_NOSIGNAL);
    recv(Socket, reinterpret_cast<void*>(&SockAddr2),size,MSG_NOSIGNAL);
    shutdown(Socket, SHUT_RDWR);
    char str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &SockAddr2.sin_addr.s_addr, str, INET_ADDRSTRLEN );
    std::cout << str << std::endl;
    std::cout << ntohs(SockAddr2.sin_port) << std::endl;
}

void tcp_server ()
{
    std::cout << "server" << std::endl <<std::flush;
    int MasterSocket = socket(
        AF_INET,
        SOCK_STREAM,
        IPPROTO_TCP);
    struct sockaddr_in MasterSockAddr;
    struct sockaddr_in SlaveSockAddr;
    unsigned int size;
    MasterSockAddr.sin_family = AF_INET;
    MasterSockAddr.sin_port = htons(12345);
    MasterSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(MasterSocket, reinterpret_cast<struct sockaddr *>(&MasterSockAddr), sizeof(MasterSockAddr));
    listen(MasterSocket,SOMAXCONN);
    bool running = true;
    while (running)
    {
        int SlaveSocket = accept(MasterSocket, reinterpret_cast<struct sockaddr *>(&SlaveSockAddr), &size);
        std::cout << SlaveSocket << std::endl;
        send(SlaveSocket, &size, sizeof(unsigned int),MSG_NOSIGNAL);
        send(SlaveSocket, reinterpret_cast<void *>(&SlaveSockAddr),size,MSG_NOSIGNAL);
        close(SlaveSocket);
    }
}
