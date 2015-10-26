//
// Created by DengYuchi on 10/20/15.
//

#ifndef EE450_SOCKET_PROJECT_SERVERB_H
#define EE450_SOCKET_PROJECT_SERVERB_H

#include "universal.h"

// TODO: Change to specified server values.

#define SERVER_NAME_STRING                      "B"
#define SERVER_UDP_PORT_NUMBER                  22646
#define SERVER_CONFIG_FILENAME                  "serverB.txt"

///////////////////////////////////////////

void read_file();
void print_server_costs();
int create_UDP_socket();
void bind_UDP_socket();
void set_up_UDP_socket();
void print_socket_address_info(int socket_descriptor, struct sockaddr_in *socket_address);
void set_up_TCP_socket();
int create_TCP_socket();
void bind_TCP_socket();
void connect_to_client_over_TCP();
void listen_to_TCP_socket();
void send_neighbor_info_over_TCP();
void receive_network_topology_over_UDP();
void close_sockets();
struct hostent * resolve_host_name(char *host_name);
void print_descriptor(int socket_descriptor);
void update_socket_info(int socket_descriptor, struct sockaddr_in *socket_address);
char * nitoa(int num, char* str, int base);
char * prepare_buffer_message(char *buffer);

#endif //EE450_SOCKET_PROJECT_SERVERB_H
