//
// Created by DengYuchi on 10/20/15.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "server.h"

int UDP_socket_descriptor;
int TCP_socket_descriptor;
int to_server_cost[NUM_SERVER];
struct sockaddr_in UDP_socket_address;
struct sockaddr_in TCP_socket_address;
struct addrinfo server_A_TCP_socket_info;
struct addrinfo server_A_UDP_socket_info;

int main() {
    set_up_UDP_socket();
    read_file();
}

void read_file() {
    FILE *file = fopen(FILENAME, "r");
    char line[LINE_LENGTH];

    if (file == NULL) {
        char error_info[MESSAGE_LENGTH];
        char *error_info_front = "Error opening file ";
        strcat(error_info, error_info_front);
        strcat(error_info, FILENAME);
        perror(error_info);
        exit(FILE_OPENING_ERROR);
    }

    while (fgets(line, LINE_LENGTH, file) != NULL) {

        char *line_part_server_name = strtok(line, LINE_TOKEN);
        char *line_part_to_server_cost = strtok(NULL, LINE_TOKEN);

        if (line_part_server_name == NULL || line_part_to_server_cost == NULL) {
            char error_info[MESSAGE_LENGTH];
            char *error_info_front = "Error opening file ";
            strcat(error_info, error_info_front);
            strcat(error_info, FILENAME);
            perror(error_info);
            exit(FILE_LINE_READING_ERROR);
        }

        to_server_cost[line_part_server_name[6] - ASCII_A] = atoi(line_part_to_server_cost);
        //printf("DEBUG: Printing each server cost...\n");
        //printf("%s - %d\n", line_part_server_name, to_server_cost[line_part_server_name[6] - ASCII_A]);
    }

    if (fclose(file)) {
        char error_info[MESSAGE_LENGTH];
        char *error_info_front = "Error closing file ";
        strcat(error_info, error_info_front);
        strcat(error_info, FILENAME);
        perror(error_info);
        exit(FILE_CLOSING_ERROR);
    }

    DEBUG ? print_server_costs() : NULL;
}

void print_server_costs() {
    int i;
    printf("==========================================\n");
    printf("DEBUG: Printing server costs...\n");
    for (i = 0; i < NUM_SERVER; i++) {
        printf("sever%c - %d\n", i + ASCII_A, to_server_cost[i]);
    }
    printf("==========================================\n\n");
}

int set_up_UDP_socket() {
    int UDP_socket_descriptor = create_UDP_socket();
    if (UDP_socket_descriptor < 0) {
        perror("cannot create UDP socket");
        exit(UDP_SOCKET_CREATION_ERROR);
    }
    bind_UDP_socket();
    // TODO Update close method;
    close(UDP_socket_descriptor);
    return (0);
}

int create_UDP_socket() {
    if ((UDP_socket_descriptor = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        return -1;
    }
    else return UDP_socket_descriptor;
}

int bind_UDP_socket() {
    memset((void *)&UDP_socket_address, 0 , sizeof(struct sockaddr_in));
    struct hostent *nunki_server_IP_address_list_raw;
    struct in_addr **nunki_server_IP_address_list;

    if ((nunki_server_IP_address_list_raw = gethostbyname(HOST_NAME)) == NULL) {
        char error_info[MESSAGE_LENGTH];
        char *error_info_front = "Error finding IP address for ";
        strcat(error_info, error_info_front);
        strcat(error_info, NUNKI_SERVER_NAME);
        perror(error_info);
        exit(CANNOT_RESOLVE_SERVER_IP_ADDRESS_ERROR);
    }

    nunki_server_IP_address_list = (struct in_addr **)nunki_server_IP_address_list_raw->h_addr_list;

    UDP_socket_address.sin_family = AF_INET;
    UDP_socket_address.sin_addr = **nunki_server_IP_address_list;
    UDP_socket_address.sin_port = htons(SERVER_A_UDP_PORT_NUMBER);

    server_A_UDP_socket_info.ai_family = UDP_socket_address.sin_family;
    server_A_UDP_socket_info.ai_addr = (struct sockaddr *)&UDP_socket_address;
    server_A_UDP_socket_info.ai_addrlen = (socklen_t)sizeof(UDP_socket_address);
    server_A_UDP_socket_info.ai_socktype = SOCK_DGRAM;

    if (bind(UDP_socket_descriptor, server_A_UDP_socket_info.ai_addr, server_A_UDP_socket_info.ai_addrlen) < 0) {
        char error_info[MESSAGE_LENGTH];
        char *error_info_front = "Error binding adresses for socket ";
        char *socket_descriptor[MESSAGE_LENGTH];
        strcat(error_info, error_info_front);
        sprintf(socket_descriptor, "%d", UDP_socket_descriptor);
        strcat(error_info, socket_descriptor);
        perror(error_info);
        exit(CANNOT_BIND_TO_SOCKET_ERROR);
    }

    DEBUG ? print_server_info() : NULL;
}

void print_server_info() {
    char *ip_addr[MESSAGE_LENGTH];
    printf("==========================================\n");
    printf("DEBUG: Bind successful...\n");
    printf("DESCRIPTOR: %d\n", UDP_socket_descriptor);
    printf("SOCKET TYPE: %d\n", server_A_UDP_socket_info.ai_socktype);
    printf("ADRESS LENGTH: %d\n", server_A_UDP_socket_info.ai_addrlen);
    printf("FAMILY: %d\n", server_A_UDP_socket_info.ai_family);
    inet_ntop(server_A_UDP_socket_info.ai_family, &(((struct sockaddr_in *)server_A_UDP_socket_info.ai_addr)->sin_addr), ip_addr, MESSAGE_LENGTH);
    printf("IP ADDRESS: %s", ip_addr);
    printf("\n");
    printf("==========================================\n\n");
}