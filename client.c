//
// Created by DengYuchi on 10/21/15.
//

#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "client.h"

int TCP_socket_descriptor;
int UDP_socket_server_A_descriptor;
int UDP_socket_server_B_descriptor;
int UDP_socket_server_C_descriptor;
int UDP_socket_server_D_descriptor;
int to_server_cost[NUM_SERVER][NUM_SERVER];
struct sockaddr_in TCP_socket_address;
struct sockaddr_in UDP_socket_server_A_address;
struct sockaddr_in UDP_socket_server_B_address;
struct sockaddr_in UDP_socket_server_C_address;
struct sockaddr_in UDP_socket_server_D_address;

int main() {
    printf("\n");
//    set_up_UDP_socket();
    set_up_TCP_socket();
//    establish_TCP_connection();
//    send_neighbor_info_over_TCP();
//    receive_network_topology_over_UDP();
    while (TRUE) {
        ;
    }
    close_sockets();
}

void set_up_TCP_socket() {
    if ((TCP_socket_descriptor = create_TCP_socket()) < 0) {
        perror("cannot create TCP socket");
        exit(TCP_SOCKET_CREATION_ERROR);
    }

    DEBUG ? print_descriptor(TCP_socket_descriptor) : NULL;

    bind_TCP_socket();

    DEBUG ? print_socket_address_info(TCP_socket_descriptor, &TCP_socket_address) : NULL;

    listen_TCP_socket();

    printf("Listnening to port...");
}

int create_TCP_socket() {
    if ((TCP_socket_descriptor = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        return -1;
    }
    else return TCP_socket_descriptor;
}

void bind_TCP_socket() {
    memset((void *)&TCP_socket_address, 0 , sizeof(struct sockaddr_in));
    struct hostent *nunki_server_IP_address_list_raw = resolve_host_name(HOST_NAME);
    struct in_addr **nunki_server_IP_address_list = (struct in_addr **)nunki_server_IP_address_list_raw->h_addr_list;

    TCP_socket_address.sin_family = AF_INET;
    TCP_socket_address.sin_addr = **nunki_server_IP_address_list;
    TCP_socket_address.sin_port = htons(CLIENT_TCP_PORT_NUMBER);

    if (bind(TCP_socket_descriptor, (struct sockaddr *)&TCP_socket_address, sizeof(TCP_socket_address)) < 0) {
        char error_info[MESSAGE_LENGTH];
        char *error_info_front = "Error binding adresses for TCP socket ";
        char *socket_descriptor[MESSAGE_LENGTH];
        strcat(error_info, error_info_front);
        sprintf(socket_descriptor, "%d", TCP_socket_descriptor);
        strcat(error_info, socket_descriptor);
        perror(error_info);
        exit(CANNOT_BIND_TO_TCP_SOCKET_ERROR);
    }

    update_socket_info(TCP_socket_descriptor, &TCP_socket_address);
}

void listen_TCP_socket() {
    if (listen(TCP_socket_descriptor, 5) < 0) {
        char error_info[MESSAGE_LENGTH];
        char *error_info_front = "Error listening to TCP socket ";
        char *socket_descriptor[MESSAGE_LENGTH];
        strcat(error_info, error_info_front);
        sprintf(socket_descriptor, "%d", TCP_socket_descriptor);
        strcat(error_info, socket_descriptor);
        perror(error_info);
        exit(CANNOT_LISTEN_TO_TCP_SOCKET_ERROR);
    }
}

struct hostent * resolve_host_name(char *host_name) {
    struct hostent *host_IP_address;
    if ((host_IP_address = gethostbyname(host_name)) == NULL) {
        char error_info[MESSAGE_LENGTH];
        char *error_info_front = "Error finding IP address for ";
        strcat(error_info, error_info_front);
        strcat(error_info, host_name);
        perror(error_info);
        exit(CANNOT_RESOLVE_HOST_IP_ADDRESS_ERROR);
    }
    else return host_IP_address;
}

void print_descriptor(int socket_descriptor) {
    printf("Socket descriptor: %d\n", socket_descriptor);
}

void update_socket_info(int socket_descriptor, struct sockaddr_in *socket_address) {
    int address_length = sizeof(socket_address);
    if (getsockname(socket_descriptor, (struct sockaddr *)socket_address, (socklen_t *)&address_length)
        < 0) {
        char socket_descriptor_str[MESSAGE_LENGTH];
        char error_info[MESSAGE_LENGTH];
        char *error_info_front = "Error getting sockect name for socket #";
        strcat(error_info, error_info_front);
        sprintf(socket_descriptor_str, "%d", socket_descriptor);
        strcat(error_info, socket_descriptor_str);
        perror(error_info);
        exit(CANNOT_GET_SOCKET_NAME_ERROR);
    }
}

void print_socket_address_info(int socket_descriptor, struct sockaddr_in *socket_address) {
    char *ip_address[MESSAGE_LENGTH];
    printf("==========================================\n");
    printf("DEBUG: Bind successful...\n");
    printf("DESCRIPTOR: %d\n", socket_descriptor);
    printf("FAMILY: %d\n", socket_address->sin_family);
    printf("ADDRESS LENGTH: %d\n", socket_address->sin_len);
    inet_ntop(socket_address->sin_family, &(socket_address->sin_addr), ip_address, MESSAGE_LENGTH);
    printf("IP ADDRESS: %s", ip_address);
    printf("\n");
    printf("PORT NUMBER: %d\n", ntohs(socket_address->sin_port));
    printf("==========================================\n\n");
}

void close_sockets() {
    // TODO Add error handling codes.
    close(TCP_socket_descriptor);
//    close(UDP_socket_server_A_descriptor);
//    close(UDP_socket_server_B_descriptor);
//    close(UDP_socket_server_C_descriptor);
//    close(UDP_socket_server_D_descriptor);
}