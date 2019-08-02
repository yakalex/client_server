#ifndef UDP_H
#define UDP_H
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

void udp_client ();

void udp_server ();

#endif // UDP_H
