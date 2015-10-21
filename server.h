//
// Created by DengYuchi on 10/20/15.
//

#ifndef EE450_SOCKET_PROJECT_CLIENT_H
#define EE450_SOCKET_PROJECT_CLIENT_H

/**
 * TODO: Change FILENAME to specified text file.
 */
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
#define CANNOT_RESOLVE_SERVER_IP_ADDRESS_ERROR  202
#define CANNOT_BIND_TO_SOCKET_ERROR             203

#define TRUE                                    1
#define FALSE                                   0
#define DEBUG                                   TRUE
#define LOCAL                                   TRUE

#define NUNKI_SERVER_NAME                       "nunki.usc.edu"
#define LOCAL_HOST                              "localhost"
#define SERVER_A_UDP_PORT_NUMBER                21646

void read_file();
void print_server_costs();
int create_UDP_socket();
int bind_UDP_socket(int UDP_socket_descriptor);
int set_up_udp_socket();
void print_server_info(int UDP_socket_descriptor);

#endif //EE450_SOCKET_PROJECT_CLIENT_H
