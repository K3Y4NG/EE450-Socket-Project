//
// Created by DengYuchi on 10/20/15.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "serverD.h"

int UDP_socket_descriptor;
int TCP_socket_descriptor;

struct sockaddr_in UDP_socket_address;
struct sockaddr_in client_UDP_socket_address;

struct sockaddr_in TCP_socket_address;
struct sockaddr_in client_TCP_socket_address;

int server_cost[NUM_SERVER];
int network_topology[NUM_SERVER][NUM_SERVER];

int main() {
    putchar('\n');
    set_up_UDP_socket();
    read_file();
    set_up_TCP_socket();
    connect_to_client_over_TCP();
    send_neighbor_info_over_TCP();
    receive_network_topology_over_UDP();
    close_sockets();
    return 0;
}

void set_up_UDP_socket() {
    if ((UDP_socket_descriptor = create_UDP_socket()) < 0) {
        perror("Error creating UDP socket");
        exit(UDP_SOCKET_CREATION_ERROR);
    }

    if (DEBUG) { debug_print_descriptor(UDP_socket_descriptor); }

    bind_UDP_socket();

    printf("The Server A has UDP port number %d and IP address %s.\n", ntohs(UDP_socket_address.sin_port),
           inet_ntoa(UDP_socket_address.sin_addr));
}

int create_UDP_socket() {
    if ((UDP_socket_descriptor = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        return -1;
    }
    else {
        int socket_option_value = 1;
        setsockopt(UDP_socket_descriptor, SOL_SOCKET, SO_REUSEADDR, &socket_option_value, sizeof(int));
        return UDP_socket_descriptor;
    }
}

void bind_UDP_socket() {
    memset((char *) &UDP_socket_address, 0, sizeof(struct sockaddr_in));
    struct hostent *nunki_server_IP_address_list_raw = resolve_host_name(HOST_NAME);
    struct in_addr **nunki_server_IP_address_list = (struct in_addr **) nunki_server_IP_address_list_raw->h_addr_list;

    UDP_socket_address.sin_family = AF_INET;
    UDP_socket_address.sin_addr = **nunki_server_IP_address_list;
    UDP_socket_address.sin_port = htons(SERVER_UDP_PORT_NUMBER);

    if (bind(UDP_socket_descriptor, (struct sockaddr *) &UDP_socket_address, sizeof(UDP_socket_address)) < 0) {
        display_error_message_int("Error binding address for UDP socket ", UDP_socket_descriptor,
                                  BIND_TO_UDP_SOCKET_ERROR);
    }
    update_socket_info(UDP_socket_descriptor, &UDP_socket_address);

    if (DEBUG) { debug_print_socket_address_info(UDP_socket_descriptor, &UDP_socket_address); }
}

void read_file() {
    FILE *file = fopen(SERVER_CONFIG_FILENAME, "r");
    char line[LINE_LENGTH];

    if (file == NULL) {
        display_error_message_string("Error opening file ", SERVER_CONFIG_FILENAME, FILE_OPENING_ERROR);
    }

    while (fgets(line, LINE_LENGTH, file) != NULL) {
        char *line_part_server_name = strtok(line, LINE_TOKEN);
        char *line_part_to_server_cost = strtok(NULL, LINE_TOKEN);
        if (line_part_server_name == NULL || line_part_to_server_cost == NULL) {
            display_error_message_string("Error reading file ", SERVER_CONFIG_FILENAME, FILE_LINE_READING_ERROR);
        }
        else {
            server_cost[line_part_server_name[6] - ASCII_A] = atoi(line_part_to_server_cost);
        }
    }

    if (fclose(file)) {
        display_error_message_string("Error closing file ", SERVER_CONFIG_FILENAME, FILE_CLOSING_ERROR);
    }

    print_server_costs();
}

void set_up_TCP_socket() {
    if ((TCP_socket_descriptor = create_TCP_socket()) < 0) {
        perror("Error creating TCP socket");
        exit(TCP_SOCKET_CREATION_ERROR);
    }

    if (DEBUG) { debug_print_descriptor(TCP_socket_descriptor); }

    bind_TCP_socket();

    if (DEBUG) { debug_print_socket_address_info(TCP_socket_descriptor, &TCP_socket_address); }

}

int create_TCP_socket() {
    if ((TCP_socket_descriptor = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        return -1;
    }
    else return TCP_socket_descriptor;
}

void bind_TCP_socket() {
    memset((char *) &TCP_socket_address, 0, sizeof(struct sockaddr_in));
    struct hostent *server_IP_address_list_raw = resolve_host_name(HOST_NAME);
    struct in_addr **server_IP_address_list = (struct in_addr **) server_IP_address_list_raw->h_addr_list;

    TCP_socket_address.sin_family = AF_INET;
    TCP_socket_address.sin_addr = **server_IP_address_list;
    TCP_socket_address.sin_port = htons(0);

    if (bind(TCP_socket_descriptor, (struct sockaddr *) &TCP_socket_address, sizeof(TCP_socket_address)) < 0) {
        display_error_message_int("Error binding address for TCP socket ", TCP_socket_descriptor,
                                  BIND_TO_TCP_SOCKET_ERROR);
    }

    update_socket_info(TCP_socket_descriptor, &TCP_socket_address);
}

void connect_to_client_over_TCP() {
    memset((char *) &client_TCP_socket_address, 0, sizeof(client_TCP_socket_address));
    struct hostent *client_IP_address_list_raw = resolve_host_name(HOST_NAME);
    struct in_addr **client_IP_address_list = (struct in_addr **) client_IP_address_list_raw->h_addr_list;

    client_TCP_socket_address.sin_family = AF_INET;
    client_TCP_socket_address.sin_addr = **client_IP_address_list;
    client_TCP_socket_address.sin_port = htons(CLIENT_TCP_PORT_NUMBER);

    if (connect(TCP_socket_descriptor, (struct sockaddr *) &client_TCP_socket_address,
                sizeof(client_TCP_socket_address)) < 0) {
        display_error_message_int("Error connecting client over TCP socket ", TCP_socket_descriptor,
                                  CONNECT_TO_CLIENT_OVER_TCP_ERROR);
    }

    if (DEBUG) { printf("DEBUG: Connection success!\n"); }
}

void send_neighbor_info_over_TCP() {
    char buffer[TCP_MESSAGE_LENGTH];

    prepare_buffer_message(buffer);

    if (DEBUG) {
        printf("==========================================\n");
        printf("DEBUG: Buffer string:\n%s\n", buffer);
        printf("==========================================\n\n");
    }

    if (send(TCP_socket_descriptor, buffer, TCP_MESSAGE_LENGTH, 0) < 0) {
        display_error_message_int("Error sending data over TCP socket ", TCP_socket_descriptor,
                                  SEND_DATA_OVER_TCP_ERROR);
    }

    if (DEBUG) { printf("DEBUG: Send data over TCP success!\n"); }

    print_send_info();
}

void receive_network_topology_over_UDP() {
    char buffer[UDP_MESSAGE_LENGTH];
    int address_length = sizeof(UDP_socket_address);
    for (; ;) {
        if (recvfrom(UDP_socket_descriptor, buffer, UDP_MESSAGE_LENGTH, 0,
                     (struct sockaddr *) &client_UDP_socket_address,
                     (socklen_t *) &address_length) <= 0) {
            display_error_message_int("Error receiving data over UDP socket ", UDP_socket_descriptor,
                                      READ_DATA_OVER_UDP_ERROR);
        }
        else {
            break;
        }
    }
    add_network_topology(buffer);
    print_receive_info();
}

void close_sockets() {
    if (close(UDP_socket_descriptor) < 0) {
        display_error_message_int("Error closing UDP socket ", UDP_socket_descriptor, UDP_SOCKET_CLOSE_ERROR);
    }
    if (close(TCP_socket_descriptor) < 0) {
        display_error_message_int("Error closing TCP socket ", TCP_socket_descriptor, TCP_SOCKET_CLOSE_ERROR);
    }
}

struct hostent *resolve_host_name(char *host_name) {
    struct hostent *host_IP_address;
    if ((host_IP_address = gethostbyname(host_name)) == NULL) {
        display_error_message_string("Error finding IP address for ", host_name, RESOLVE_HOST_IP_ADDRESS_ERROR);
    }
    return host_IP_address;
}

void update_socket_info(int socket_descriptor, struct sockaddr_in *socket_address) {
    int address_length = sizeof(socket_address);
    if (getsockname(socket_descriptor, (struct sockaddr *) socket_address, (socklen_t *) &address_length) < 0) {
        display_error_message_int("Error getting sockect name for socket ", socket_descriptor, GET_SOCKET_NAME_ERROR);
    }
}

char *nitoa(int number, char *string, int base) {
    int i = 0, j = 0;
    int is_negative = FALSE;
    char temp;

    if (number == 0) {
        string[i++] = '0';
        string[i] = '\0';
        while (i < MESSAGE_PART_LENGTH - 1) {
            string[i++] = '0';
        }
        string[i] = '\0';
        return string;
    }

    if (number < 0 && base == 10) {
        is_negative = TRUE;
        number = -number;
    }

    while (number != 0) {
        int remainder = number % base;
        string[i++] = (char) ((remainder > 9) ? (remainder - 10) + ASCII_a : remainder + ASCII_0);
        number = number / base;
    }

    if (is_negative)
        string[i++] = '-';

    while (i < MESSAGE_PART_LENGTH - 1) {
        string[i++] = '0';
    }

    string[i] = '\0';

    for (j = 0; j < MESSAGE_PART_LENGTH / 2; j++) {
        temp = string[j];
        string[j] = string[MESSAGE_PART_LENGTH - j - 2];
        string[MESSAGE_PART_LENGTH - j - 2] = temp;
    }
    return string;
}

char *prepare_buffer_message(char *buffer) {
    char cost[MESSAGE_PART_LENGTH];
    int i, j;
    for (i = 0; i < NUM_SERVER; i++) {
        nitoa(server_cost[i], cost, 10);
        for (j = 0; j < MESSAGE_PART_LENGTH - 1; j++) {
            buffer[i * (MESSAGE_PART_LENGTH - 1) + j] = cost[j];
        }
    }
    buffer[TCP_MESSAGE_LENGTH - 2] = SERVER_NAME_CHAR;
    buffer[TCP_MESSAGE_LENGTH - 1] = '\0';
    return buffer;
}

void add_network_topology(char *buffer) {
    int i, j, k;
    for (i = 0; i < NUM_SERVER; i++) {
        for (j = i + 1; j < NUM_SERVER; j++) {
            char cost_string[MESSAGE_PART_LENGTH];
            int cost;
            for (k = 0; k < MESSAGE_PART_LENGTH - 1; k++) {
                cost_string[k] = buffer[(i * NUM_SERVER + j) * (MESSAGE_PART_LENGTH - 1) + k];
            }
            cost_string[k] = '\0';
            cost = atoi(cost_string);
            if (cost) {
                network_topology[i][j] = cost;
                network_topology[j][i] = cost;
            }
        }
    }
}

void print_server_costs() {
    int i;

    putchar('\n');
    printf("The Server %c has the following neighbor information:\n", SERVER_NAME_CHAR);
    printf("Neighbor------Cost\n");
    for (i = 0; i < NUM_SERVER; i++) {
        int cost = server_cost[i];
        if (cost) {
            printf("sever%c %10d\n", i + ASCII_A, cost);
        }
    }
}

void print_send_info() {
    putchar('\n');
    printf("The Server %c finishes sending its neighbor information to the Client with "
                   "TCP port number %d and IP address %s.\n", SERVER_NAME_CHAR,
           htons(client_TCP_socket_address.sin_port),
           inet_ntoa(client_TCP_socket_address.sin_addr));
    putchar('\n');
    printf("For this connection with the Client, the Server %c has T​CP port number %d and IP address %s.\n",
           SERVER_NAME_CHAR, htons(TCP_socket_address.sin_port), inet_ntoa(TCP_socket_address.sin_addr));
}

void print_receive_info() {
    putchar('\n');
    printf("The server %c has received the network topology from the Client with "
                   "UDP port number %d and IP address %s as follows:\n", SERVER_NAME_CHAR,
           htons(client_UDP_socket_address.sin_port), inet_ntoa(client_UDP_socket_address.sin_addr));
    print_edge_cost();
    putchar('\n');
    printf("For this connection with Client, the Server %c has UDP port number %d and IP address %s.\n",
           SERVER_NAME_CHAR, htons(UDP_socket_address.sin_port), inet_ntoa(UDP_socket_address.sin_addr));
    putchar('\n');
}

void print_edge_cost() {
    int i, j;

    printf("Edge------Cost\n");
    for (i = 0; i < NUM_SERVER; i++) {
        for (j = i + 1; j < NUM_SERVER; j++) {
            int cost = network_topology[i][j];
            if (cost) {
                printf("%c%c  %10d\n", i + ASCII_A, j + ASCII_A, cost);
            }
        }
    }
}

void display_error_message_int(char *error_info_front, int socket_descriptor, int error_number) {
    char error_info[ERROR_MESSAGE_LENGTH];
    char socket_descriptor_string[ERROR_MESSAGE_LENGTH];
    strcat(error_info, error_info_front);
    sprintf(socket_descriptor_string, "%d", socket_descriptor);
    strcat(error_info, socket_descriptor_string);
    perror(error_info);
    exit(error_number);
}

void display_error_message_string(char *error_info_front, char *error_info_back, int error_number) {
    char error_info[ERROR_MESSAGE_LENGTH];
    strcat(error_info, error_info_front);
    strcat(error_info, error_info_back);
    perror(error_info);
    exit(error_number);
}

void debug_print_descriptor(int socket_descriptor) {
    printf("DEBUG: Socket descriptor: %d\n", socket_descriptor);
}

void debug_print_socket_address_info(int socket_descriptor, struct sockaddr_in *socket_address) {
    char ip_address[ERROR_MESSAGE_LENGTH];
    printf("==========================================\n");
    printf("DEBUG: Socket Info\n");
    printf("DESCRIPTOR: %d\n", socket_descriptor);
    printf("FAMILY: %d\n", socket_address->sin_family);
    printf("ADDRESS LENGTH: %d\n", socket_address->sin_len);
    inet_ntop(socket_address->sin_family, &(socket_address->sin_addr), ip_address, ERROR_MESSAGE_LENGTH);
    printf("IP ADDRESS: %s", ip_address);
    printf("\n");
    printf("PORT NUMBER: %d\n", ntohs(socket_address->sin_port));
    printf("==========================================\n\n");
}
