#ifndef RAW_UDP_H
#define RAW_UDP_H
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <string.h>

void raw_udp_client ();

void raw_udp_server ();

#endif // RAW_UDP_H
