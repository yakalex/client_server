#ifndef TCP_H
#define TCP_H
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>

void tcp_client ();

void tcp_server ();

#endif // TCP_H
