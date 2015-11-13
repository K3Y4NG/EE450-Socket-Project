//
// Created by DengYuchi on 10/20/15.
//

#ifndef EE450_SOCKET_PROJECT_SERVERB_H
#define EE450_SOCKET_PROJECT_SERVERB_H

#include "universal.h"


/**
 * Change to specific server values.
 */
#define SERVER_NAME_CHAR                        'B'
#define SERVER_UDP_PORT_NUMBER                  22646
#define SERVER_CONFIG_FILENAME                  "serverB.txt"

///////////////////////////////////////////

/**
 * Main flow functions.
 */
void set_up_UDP_socket();
int create_UDP_socket();
void bind_UDP_socket();
void read_file();
void set_up_TCP_socket();
int create_TCP_socket();
void connect_to_client_over_TCP();
void send_neighbor_info_over_TCP();
void receive_network_topology_over_UDP();
void close_sockets();


/**
 * Utility functions.
 */
struct hostent * resolve_host_name(char *host_name);
void update_socket_info(int socket_descriptor, struct sockaddr_in *socket_address);
char * nitoa(int number, char*string, int base);
char * prepare_buffer_message(char *buffer);
void add_network_topology(char *buffer);


/*
 * On-screen message functions.
 */
void print_server_costs();
void print_send_info();
void print_receive_info();
void print_edge_cost();


/**
 * On-screen error display functions.
 */
void display_error_message_int(char *error_info_front, int socket_descriptor, int error_number);
void display_error_message_string(char *error_info_front, char *error_info_back, int error_number);

#endif //EE450_SOCKET_PROJECT_SERVERB_H
