//
// Created by DengYuchi on 10/20/15.
//

#ifndef EE450_SOCKET_PROJECT_SERVERA_H
#define EE450_SOCKET_PROJECT_SERVERA_H

#include "universal.h"

// TODO: Change to specified server values.

#define SERVER_NAME_CHAR                        'A'
#define SERVER_UDP_PORT_NUMBER                  21646
#define SERVER_CONFIG_FILENAME                  "serverA.txt"

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
void send_neighbor_info_over_TCP();
void receive_network_topology_over_UDP();
void close_sockets();
struct hostent * resolve_host_name(char *host_name);
void print_descriptor(int socket_descriptor);
void update_socket_info(int socket_descriptor, struct sockaddr_in *socket_address);
char * nitoa(int number, char*string, int base);
char * prepare_buffer_message(char *buffer);
void print_network_topology(char *buffer);
void display_error_message_int(char *error_info_front, int socket_descriptor, int error_number);
void display_error_message_string(char *error_info_front, char *error_info_back, int error_number);

#endif //EE450_SOCKET_PROJECT_SERVERA_H
