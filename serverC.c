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
#include "serverC.h"

int UDP_socket_descriptor;
int TCP_socket_descriptor;
int to_server_cost[NUM_SERVER];
struct sockaddr_in UDP_socket_address;
struct sockaddr_in TCP_socket_address;
struct sockaddr_in client_TCP_socket_address;

int main() {
    printf("\n");
//    set_up_UDP_socket();
    read_file();
    set_up_TCP_socket();
    connect_to_client_over_TCP();
    if (DEBUG) { printf("Connection success!\n"); }
    send_neighbor_info_over_TCP();
    if (DEBUG) { printf("Send data over TCP success!\n"); }
//    receive_network_topology_over_UDP();
//    while (TRUE) {
//        ;
//    }
    close_sockets();
}

void read_file() {
    FILE *file = fopen(SERVER_CONFIG_FILENAME, "r");
    char line[LINE_LENGTH];

    if (file == NULL) {
        char error_info[MESSAGE_LENGTH];
        char *error_info_front = "Error opening file ";
        strcat(error_info, error_info_front);
        strcat(error_info, SERVER_CONFIG_FILENAME);
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
            strcat(error_info, SERVER_CONFIG_FILENAME);
            perror(error_info);
            exit(FILE_LINE_READING_ERROR);
        }

        to_server_cost[line_part_server_name[6] - ASCII_A] = atoi(line_part_to_server_cost);
    }

    if (fclose(file)) {
        char error_info[MESSAGE_LENGTH];
        char *error_info_front = "Error closing file ";
        strcat(error_info, error_info_front);
        strcat(error_info, SERVER_CONFIG_FILENAME);
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

void set_up_UDP_socket() {
    if ((UDP_socket_descriptor = create_UDP_socket()) < 0) {
        perror("cannot create UDP socket");
        exit(UDP_SOCKET_CREATION_ERROR);
    }

    DEBUG ? print_descriptor(UDP_socket_descriptor) : NULL;

    bind_UDP_socket();
}

int create_UDP_socket() {
    if ((UDP_socket_descriptor = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        return -1;
    }
    else {
//        int socket_option_value = 1;
//        setsockopt(UDP_socket_descriptor, SOL_SOCKET, SO_REUSEADDR, &socket_option_value, sizeof(int));
        return UDP_socket_descriptor;
    }
}

void bind_UDP_socket() {
    memset((void *)&UDP_socket_address, 0 , sizeof(struct sockaddr_in));
    struct hostent *nunki_server_IP_address_list_raw = resolve_host_name(HOST_NAME);
    struct in_addr **nunki_server_IP_address_list = (struct in_addr **)nunki_server_IP_address_list_raw->h_addr_list;

    UDP_socket_address.sin_family = AF_INET;
    UDP_socket_address.sin_addr = **nunki_server_IP_address_list;
    UDP_socket_address.sin_port = htons(SERVER_UDP_PORT_NUMBER);

    if (bind(UDP_socket_descriptor, (struct sockaddr *)&UDP_socket_address, sizeof(UDP_socket_address)) < 0) {
        char error_info[MESSAGE_LENGTH];
        char *error_info_front = "Error binding adresses for UDP socket ";
        char *socket_descriptor[MESSAGE_LENGTH];
        strcat(error_info, error_info_front);
        sprintf(socket_descriptor, "%d", UDP_socket_descriptor);
        strcat(error_info, socket_descriptor);
        perror(error_info);
        exit(CANNOT_BIND_TO_UDP_SOCKET_ERROR);
    }

    update_socket_info(UDP_socket_descriptor, &UDP_socket_address);

    DEBUG ? print_socket_address_info(UDP_socket_descriptor, &UDP_socket_address) : NULL;
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

void set_up_TCP_socket() {
    if ((TCP_socket_descriptor = create_TCP_socket()) < 0) {
        perror("cannot create TCP socket");
        exit(TCP_SOCKET_CREATION_ERROR);
    }

    DEBUG ? print_descriptor(TCP_socket_descriptor) : NULL;

    bind_TCP_socket();

    DEBUG ? print_socket_address_info(TCP_socket_descriptor, &TCP_socket_address) : NULL;

}

int create_TCP_socket() {
    if ((TCP_socket_descriptor = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        return -1;
    }
    else return TCP_socket_descriptor;
}

void bind_TCP_socket() {
    memset((void *)&TCP_socket_address, 0 , sizeof(struct sockaddr_in));
    struct hostent *server_IP_address_list_raw = resolve_host_name(HOST_NAME);
    struct in_addr **server_IP_address_list = (struct in_addr **)server_IP_address_list_raw->h_addr_list;

    TCP_socket_address.sin_family = AF_INET;
    TCP_socket_address.sin_addr = **server_IP_address_list;
    TCP_socket_address.sin_port = htons(0);

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

void connect_to_client_over_TCP() {
    memset((void *)&client_TCP_socket_address, 0, sizeof(client_TCP_socket_address));
    struct hostent *client_IP_address_list_raw = resolve_host_name(HOST_NAME);
    struct in_addr **client_IP_address_list = (struct in_addr **)client_IP_address_list_raw->h_addr_list;

    client_TCP_socket_address.sin_family = AF_INET;
    client_TCP_socket_address.sin_addr = **client_IP_address_list;
    client_TCP_socket_address.sin_port = htons(CLIENT_TCP_PORT_NUMBER);

    if (connect(TCP_socket_descriptor, (struct sockaddr *)&client_TCP_socket_address, sizeof(client_TCP_socket_address)) < 0) {
        char error_info[MESSAGE_LENGTH];
        char *error_info_front = "Error connecting client over TCP socket ";
        char *socket_descriptor[MESSAGE_LENGTH];
        strcat(error_info, error_info_front);
        sprintf(socket_descriptor, "%d", TCP_socket_descriptor);
        strcat(error_info, socket_descriptor);
        perror(error_info);
        exit(CONNECT_TO_CLIENT_OVER_TCP_ERROR);
    }
};

void listen_to_TCP_socket() {
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

void send_neighbor_info_over_TCP() {
    char buffer[TCP_MESSAGE_LENGTH];
//    char test_buffer[TCP_MESSAGE_LENGTH];
//    int i;
    prepare_buffer_message(buffer);
    printf("==========================================\n");
    if (DEBUG) { printf("DEBUG: Buffer string:\n%s\n", buffer); }
    printf("==========================================\n");

    if (send(TCP_socket_descriptor, buffer, TCP_MESSAGE_LENGTH, 0) < 0) {
        char error_info[MESSAGE_LENGTH];
        char *error_info_front = "Error sending data over TCP socket";
        strcat(error_info, error_info_front);
        strcat(error_info, TCP_socket_descriptor);
        perror(error_info);
        exit(CANNOT_SEND_DATA_OVER_TCP_ERROR);
    }

//    /**
//     * Test buffer
//     */
//
//    sleep(10);
//
//    for(i = 0; i < TCP_MESSAGE_LENGTH - 1; i++) {
//        test_buffer[i] = '1';
//    }
//
//    if (send(TCP_socket_descriptor, test_buffer, TCP_MESSAGE_LENGTH, 0) < 0) {
//        char error_info[MESSAGE_LENGTH];
//        char *error_info_front = "Error sending test data over TCP socket";
//        strcat(error_info, error_info_front);
//        strcat(error_info, TCP_socket_descriptor);
//        perror(error_info);
//        exit(CANNOT_SEND_DATA_OVER_TCP_ERROR);
//    }
}

char * prepare_buffer_message(char *buffer) {
    char cost[MESSAGE_PART_LENGTH];
    int i, j;
    for (i = 0; i < NUM_SERVER; i++) {
        nitoa(to_server_cost[i], cost, 10);
        for (j = 0; j < MESSAGE_PART_LENGTH - 1; j++) {
            buffer[i * (MESSAGE_PART_LENGTH - 1) + j] = cost[j];
        }
    }
    strcat(buffer, SERVER_NAME_STRING);
}

void receive_network_topology_over_UDP() {

}

void close_sockets() {
    // TODO Add error handling codes.
    close(UDP_socket_descriptor);
    close(TCP_socket_descriptor);
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

char *nitoa(int num, char *str, int base) {
    int i = 0, j = 0;
    int isNegative = FALSE;
    char temp;

    /* Handle 0 explicitely, otherwise empty string is printed for 0 */
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        while (i < MESSAGE_PART_LENGTH - 1) {
            str[i++] = '0';
        }
        str[i] = '\0'; // Append string terminator
        return str;
    }

    // In standard itoa(), negative numbers are handled only with
    // base 10. Otherwise numbers are considered unsigned.
    if (num < 0 && base == 10) {
        isNegative = TRUE;
        num = -num;
    }

    // Process individual digits
    while (num != 0) {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }

    // If number is negative, append '-'
    if (isNegative)
        str[i++] = '-';

    while (i < MESSAGE_PART_LENGTH - 1) {
        str[i++] = '0';
    }

    str[i] = '\0'; // Append string terminator

    // Reverse the string
    for (j = 0; j < MESSAGE_PART_LENGTH / 2; j++) {
        temp = str[j];
        str[j] = str[MESSAGE_PART_LENGTH - j - 2];
        str[MESSAGE_PART_LENGTH - j - 2] = temp;
    }
    return str;
}