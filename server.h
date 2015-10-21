//
// Created by DengYuchi on 10/20/15.
//

#ifndef EE450_SOCKET_PROJECT_CLIENT_H
#define EE450_SOCKET_PROJECT_CLIENT_H

/**
 * TODO: Change FILENAME to specified text file. Check DEBUG and LOCAL constants.
 */
#define TRUE                                    1
#define FALSE                                   0
#define DEBUG                                   TRUE
#define LOCAL                                   TRUE

#define FILENAME                                "serverA.txt"
#define MESSAGE_LENGTH                          256
#define LINE_LENGTH                             40
#define NUM_SERVER                              4

#define LINE_TOKEN                              " "
#define ASCII_A                                 65

#define FILE_OPENING_ERROR                      101
#define FILE_LINE_READING_ERROR                 102
#define FILE_CLOSING_ERROR                      103

#define UDP_SOCKET_CREATION_ERROR               201
#define TCP_SOCKET_CREATION_ERROR               202
#define CANNOT_RESOLVE_HOST_IP_ADDRESS_ERROR    203
#define CANNOT_BIND_TO_UDP_SOCKET_ERROR         204
#define CANNOT_BIND_TO_TCP_SOCKET_ERROR         205
#define CANNOT_GET_SOCKET_NAME_ERROR            206

#define NUNKI_SERVER_NAME                       "nunki.usc.edu"
#define LOCAL_HOST                              "localhost"
#define HOST_NAME                               LOCAL ? LOCAL_HOST : NUNKI_SERVER_NAME
#define SERVER_A_UDP_PORT_NUMBER                21646
#define SERVER_A_UDP_PORT_NUMBER_STR            "21646"

void read_file();
void print_server_costs();
int create_UDP_socket();
void bind_UDP_socket();
void set_up_UDP_socket();
void print_socket_address_info(int socket_descriptor, struct sockaddr_in *socket_address);
void set_up_TCP_socket();
int create_TCP_socket();
void bind_TCP_socket();
void establish_TCP_connection();
void send_neighbor_info_over_TCP();
void receive_network_topology_over_UDP();
void close_sockets();
struct hostent * resolve_host_name(char *host_name);
void print_descriptor(int socket_descriptor);
void update_socket_info(int socket_descriptor, struct sockaddr_in *socket_address);

#endif //EE450_SOCKET_PROJECT_CLIENT_H
