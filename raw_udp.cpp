#include <raw_udp.h>

void raw_send (int fd, const void * buf, size_t n, struct sockaddr_in * addr, socklen_t  addr_len, unsigned short senderPort)
{
    char* udp_packet = new char[sizeof(struct udphdr) + n];
    struct udphdr* UdpHeader = reinterpret_cast<struct udphdr *>(udp_packet);
    char * data = udp_packet + sizeof (struct udphdr);
    memcpy (data, buf, n);
    UdpHeader -> len = htons( sizeof (struct udphdr) + n);
    UdpHeader -> dest = addr -> sin_port;
    UdpHeader ->source = htons(senderPort);
    UdpHeader -> check = 0;
    sendto (fd, udp_packet, sizeof (struct udphdr) + n, MSG_NOSIGNAL, reinterpret_cast<struct sockaddr *> (addr), addr_len);
    delete [] udp_packet;
}

void client_raw_recv (int fd, void * buf, size_t n, struct sockaddr_in * addr, socklen_t * addr_len )
{
    auto port = addr ->sin_port;
    char* udp_packet = new char[sizeof(struct udphdr) +sizeof (struct iphdr)+ n];
    char * data = udp_packet + sizeof (struct udphdr) +sizeof (struct iphdr);
    struct udphdr* UdpHeader = reinterpret_cast<struct udphdr *>(udp_packet + sizeof (struct iphdr));
    do
    {
        recvfrom(fd, udp_packet, sizeof(struct udphdr) + sizeof (struct iphdr) + n, MSG_NOSIGNAL, reinterpret_cast<struct sockaddr *> (addr), addr_len);
    }
    while (UdpHeader ->source != port);
    memcpy (buf, data, n);
    delete [] udp_packet;
}

unsigned short server_raw_recv (int fd, void * buf, size_t n, struct sockaddr_in * addr, socklen_t * addr_len, unsigned short serverPort )
{
    auto port = htons(serverPort);
    char* udp_packet = new char[sizeof(struct udphdr) +sizeof (struct iphdr)+ n];
    char * data = udp_packet + sizeof (struct udphdr) +sizeof (struct iphdr);
    struct udphdr* UdpHeader = reinterpret_cast<struct udphdr *>(udp_packet + sizeof (struct iphdr));
    do
    {
        recvfrom(fd, udp_packet, sizeof(struct udphdr) + sizeof (struct iphdr) + n, MSG_NOSIGNAL, reinterpret_cast<struct sockaddr *> (addr), addr_len);
    }
    while (UdpHeader ->dest != port);
    memcpy (buf, data, n);
    delete [] udp_packet;
    return UdpHeader ->source;
}

void raw_udp_server ()
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
        SOCK_RAW,
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

        SlaveSockAddr.sin_port = server_raw_recv (MasterSocket, &in, sizeof(int), &SlaveSockAddr, &sizeofSlaveSockAddr, port);
        {
            raw_send(MasterSocket, &sizeofSlaveSockAddr, sizeof(unsigned int),&SlaveSockAddr, sizeofSlaveSockAddr, port);
            raw_send(MasterSocket, reinterpret_cast<void *>(&SlaveSockAddr),sizeofSlaveSockAddr, &SlaveSockAddr, sizeofSlaveSockAddr, port);
        }
    }
}

void raw_udp_client ()
{
    std::cout << "Client" << std::endl;
    std::cout << "Input Ip-address: ";
    std::string addr;
    std::cin >> addr;
    std::cout << "Input server port: ";
    unsigned short port;
    std::cin >> port;
    std::cout << "Input client port: ";
    unsigned short clientPort;
    std::cin >> clientPort;
    int Socket = socket(
        AF_INET,
        SOCK_RAW,
        IPPROTO_UDP);

    struct sockaddr_in SockAddr;
    struct sockaddr_in SockAddr2;
    SockAddr.sin_family = AF_INET;
    SockAddr.sin_port = htons(port);
    inet_pton(AF_INET,addr.c_str(), &SockAddr.sin_addr.s_addr);
    //char buffer [sizeof];
    int in = 5;
    unsigned int size;
    unsigned int sizeofSockAddr;
    {
        raw_send(Socket, &in, sizeof(int), &SockAddr, sizeof (SockAddr), clientPort);
        client_raw_recv(Socket, &size, sizeof(unsigned int),  &SockAddr, &sizeofSockAddr);
        SockAddr.sin_family = AF_INET;
        SockAddr.sin_port = htons(port);
        client_raw_recv(Socket, reinterpret_cast<void*>(&SockAddr2),size, &SockAddr, &sizeofSockAddr);
    }
    close(Socket);
    char str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &SockAddr2.sin_addr.s_addr, str, INET_ADDRSTRLEN );
    std::cout << str << std::endl;
    std::cout << ntohs(SockAddr2.sin_port) << std::endl;
}
