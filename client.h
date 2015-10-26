//
// Created by DengYuchi on 10/21/15.
//

#ifndef EE450_SOCKET_PROJECT_CLIENT_H
#define EE450_SOCKET_PROJECT_CLIENT_H

#include "universal.h"

#define CLIENT_TCP_PORT_NUMBER                  25646

///////////////////////////////////////////

void set_up_TCP_socket();
int create_TCP_socket();
void bind_TCP_socket();
void listen_to_TCP_socket();
void establish_TCP_connection();
void receive_neighbor_info_over_TCP();
struct hostent * resolve_host_name(char *host_name);
void close_sockets();
void print_descriptor(int socket_descriptor);
void update_socket_info(int socket_descriptor, struct sockaddr_in *socket_address);
void print_socket_address_info(int socket_descriptor, struct sockaddr_in *socket_address);

#endif //EE450_SOCKET_PROJECT_CLIENT_H
