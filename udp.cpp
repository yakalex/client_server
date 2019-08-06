#include "udp.h"

void udp_client (bool is_connet)
{
    std::cout << "Client" << std::endl;
    std::cout << "Input Ip-address: ";
    std::string addr;
    std::cin >> addr;
    std::cout << "Input port: ";
    unsigned short port;
    std::cin >> port;
    int Socket = socket(
        AF_INET,
        SOCK_DGRAM,
        IPPROTO_UDP);
    struct sockaddr_in SockAddr;
    struct sockaddr_in SockAddr2;
    SockAddr.sin_family = AF_INET;
    SockAddr.sin_port = htons(port);
    inet_pton(AF_INET,addr.c_str(), &SockAddr.sin_addr.s_addr);
    //SockAddr.sin_addr.s_addr = inet_addr(addr.c_str());
    int in = 1;
    unsigned int size;
    unsigned int sizeofSockAddr;
    if (is_connet)
    {
        connect (Socket, reinterpret_cast<struct sockaddr *> (&SockAddr), sizeof(SockAddr));
        send(Socket, &in, sizeof(int), MSG_NOSIGNAL);
        recv(Socket, &size, sizeof(unsigned int),MSG_NOSIGNAL);
        recv(Socket, reinterpret_cast<void*>(&SockAddr2),size,MSG_NOSIGNAL);
    }
    else
    {
        sendto(Socket, &in, sizeof(int), MSG_NOSIGNAL, reinterpret_cast<struct sockaddr *> (&SockAddr), sizeof(SockAddr));
        recvfrom(Socket, &size, sizeof(unsigned int),MSG_NOSIGNAL, reinterpret_cast<struct sockaddr *> (&SockAddr), &sizeofSockAddr);
        recvfrom(Socket, reinterpret_cast<void*>(&SockAddr2),size,MSG_NOSIGNAL, reinterpret_cast<struct sockaddr *> (&SockAddr), &sizeofSockAddr);
    }
    close(Socket);
    char str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &SockAddr2.sin_addr.s_addr, str, INET_ADDRSTRLEN );
    std::cout << str << std::endl;
    std::cout << ntohs(SockAddr2.sin_port) << std::endl;
}

void udp_server (bool is_connect)
{
    std::cout << "Server" << std::endl << std::flush;
    std::cout << "Input Ip-address: ";
    std::string addr;
    std::cin >> addr;
    std::cout << "Input port: ";
    unsigned short port;
    std::cin >> port;
    int MasterSocket = socket(
        AF_INET,
        SOCK_DGRAM,
        IPPROTO_UDP);
    struct sockaddr_in MasterSockAddr;
    struct sockaddr_in SlaveSockAddr;
    MasterSockAddr.sin_family = AF_INET;
    MasterSockAddr.sin_port = htons(port);
    inet_pton(AF_INET,addr.c_str(), &MasterSockAddr.sin_addr.s_addr);
    bind(MasterSocket, reinterpret_cast<struct sockaddr *>(&MasterSockAddr), sizeof(MasterSockAddr));
    socklen_t sizeofSlaveSockAddr;
    int in;
    //listen(MasterSocket,SOMAXCONN);
    bool running = true;
    while (running)
    {
        //int SlaveSocket;// = accept(MasterSocket, reinterpret_cast<struct sockaddr *>(&SlaveSockAddr), &size);
        //std::cout << SlaveSocket << std::endl;

        recvfrom (MasterSocket, &in, sizeof(int), MSG_NOSIGNAL, reinterpret_cast<struct sockaddr *>(&SlaveSockAddr), &sizeofSlaveSockAddr);
        if (is_connect)
        {
            connect(MasterSocket, reinterpret_cast<struct sockaddr *> (&SlaveSockAddr), sizeofSlaveSockAddr);
            send(MasterSocket, &sizeofSlaveSockAddr, sizeof(unsigned int),MSG_NOSIGNAL);//, reinterpret_cast<struct sockaddr *> (&SlaveSockAddr), sizeofSlaveSockAddr);
            send(MasterSocket, reinterpret_cast<void *>(&SlaveSockAddr),sizeofSlaveSockAddr,MSG_NOSIGNAL);//, reinterpret_cast<struct sockaddr *> (&SlaveSockAddr), sizeofSlaveSockAddr);
            close(MasterSocket);
            MasterSocket = socket(
                AF_INET,
                SOCK_DGRAM,
                IPPROTO_UDP);
            bind(MasterSocket, reinterpret_cast<struct sockaddr *>(&MasterSockAddr), sizeof(MasterSockAddr));
        }
        else
        {
            sendto(MasterSocket, &sizeofSlaveSockAddr, sizeof(unsigned int),MSG_NOSIGNAL, reinterpret_cast<struct sockaddr *> (&SlaveSockAddr), sizeofSlaveSockAddr);
            sendto(MasterSocket, reinterpret_cast<void *>(&SlaveSockAddr),sizeofSlaveSockAddr,MSG_NOSIGNAL, reinterpret_cast<struct sockaddr *> (&SlaveSockAddr), sizeofSlaveSockAddr);
        }
    }
}
