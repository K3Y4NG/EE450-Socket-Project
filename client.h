//
// Created by DengYuchi on 10/21/15.
//

#ifndef EE450_SOCKET_PROJECT_CLIENT_H
#define EE450_SOCKET_PROJECT_CLIENT_H

#include "universal.h"

#define CLIENT_TCP_PORT_NUMBER                  25646

///////////////////////////////////////////

void set_up_UDP_socket();

int create_UDP_socket();

void bind_UDP_socket();

void set_up_TCP_socket();

int create_TCP_socket();

void bind_TCP_socket();

void listen_to_TCP_socket();

void establish_TCP_connection();

void receive_neighbor_info_over_TCP();

struct hostent *resolve_host_name(char *host_name);

void close_sockets();

void print_descriptor(int socket_descriptor);

void update_socket_info(int socket_descriptor, struct sockaddr_in *socket_address);

void print_socket_address_info(int socket_descriptor, struct sockaddr_in *socket_address);

void add_to_server_cost(char *buffer);

void calculate_network_MST();

void send_network_topology_over_UDP();

void prepare_buffer_message(char *buffer);

int min_key(int key[], int not_included[]);

char *nitoa(int num, char *str, int base);

void display_error_message_int(char *error_info_front, int socket_descriptor, int error_number);

void display_error_message_string(char *error_info_front, char *error_info_back, int error_number);

void print_receive_info(char server_name, struct sockaddr_in *server_TCP_socket_address);

void print_send_info(char server_name, struct sockaddr_in *server_UDP_socket_address);

void print_network_MST_info();

void print_edge_cost(int edge_cost[NUM_SERVER][NUM_SERVER]);

long calculate_network_MST_cost();

#endif //EE450_SOCKET_PROJECT_CLIENT_H
