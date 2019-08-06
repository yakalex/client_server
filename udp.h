#ifndef UDP_H
#define UDP_H
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

void udp_client (bool is_connect);

void udp_server (bool is_connect);

#endif // UDP_H
