//
// Created by DengYuchi on 10/21/15.
//

#ifndef EE450_SOCKET_PROJECT_UNIVERSAL_H
#define EE450_SOCKET_PROJECT_UNIVERSAL_H

#define TRUE                                    1
#define FALSE                                   0

// TODO: Check DEBUG and LOCAL constants.

#define DEBUG                                   TRUE
#define LOCAL                                   TRUE

/////////////////////////////////////////

#define SERVER_A_UDP_PORT_NUMBER                21646
#define SERVER_B_UDP_PORT_NUMBER                22646
#define SERVER_C_UDP_PORT_NUMBER                23646
#define SERVER_D_UDP_PORT_NUMBER                24646
#define CLIENT_TCP_PORT_NUMBER                  25646

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
#define CANNOT_LISTEN_TO_TCP_SOCKET_ERROR       207
#define CONNECT_TO_CLIENT_OVER_TCP_ERROR        208

#define NUNKI_SERVER_NAME                       "nunki.usc.edu"
#define LOCAL_HOST                              "localhost"
#define HOST_NAME                               LOCAL ? LOCAL_HOST : NUNKI_SERVER_NAME

#endif //EE450_SOCKET_PROJECT_UNIVERSAL_H
