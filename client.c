//
// Created by DengYuchi on 10/21/15.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/errno.h>
#include <limits.h>
#include "client.h"

int TCP_socket_descriptor;
int UDP_socket_descriptor;

int server_A_TCP_socket_descriptor;
int server_B_TCP_socket_descriptor;
int server_C_TCP_socket_descriptor;
int server_D_TCP_socket_descriptor;

struct sockaddr_in TCP_socket_address;
struct sockaddr_in UDP_socket_address;

struct sockaddr_in server_A_TCP_socket_address;
struct sockaddr_in server_B_TCP_socket_address;
struct sockaddr_in server_C_TCP_socket_address;
struct sockaddr_in server_D_TCP_socket_address;
struct sockaddr_in server_UDP_socket_address;

int server_cost[NUM_SERVER][NUM_SERVER];
int network_MST[NUM_SERVER][NUM_SERVER];

extern int errno;

int main() {
    putchar('\n');
    set_up_UDP_socket();
    set_up_TCP_socket();
    establish_TCP_connection();
    receive_neighbor_info_over_TCP();
    send_network_topology_over_UDP();
    calculate_network_MST();
    close_sockets();
    return 0;
}

void set_up_UDP_socket() {
    if ((UDP_socket_descriptor = create_UDP_socket()) < 0) {
        perror("Error creating UDP socket");
        exit(UDP_SOCKET_CREATION_ERROR);
    }
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

void set_up_TCP_socket() {
    if ((TCP_socket_descriptor = create_TCP_socket()) < 0) {
        perror("Error creating TCP socket");
        exit(TCP_SOCKET_CREATION_ERROR);
    }
    bind_TCP_socket();
    printf("The Client has TCP port number %d and IP address %s.\n", ntohs(TCP_socket_address.sin_port),
           inet_ntoa(TCP_socket_address.sin_addr));
    listen_to_TCP_socket();
}

int create_TCP_socket() {
    if ((TCP_socket_descriptor = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        return -1;
    }
    else {
        int socket_option_value = 1;
        setsockopt(TCP_socket_descriptor, SOL_SOCKET, SO_REUSEADDR, &socket_option_value, sizeof(int));
        return TCP_socket_descriptor;
    }
}

void bind_TCP_socket() {
    memset((char *) &TCP_socket_address, 0, sizeof(struct sockaddr_in));
    struct hostent *nunki_server_IP_address_list_raw = resolve_host_name(HOST_NAME);
    struct in_addr **nunki_server_IP_address_list = (struct in_addr **) nunki_server_IP_address_list_raw->h_addr_list;
    TCP_socket_address.sin_family = AF_INET;
    TCP_socket_address.sin_addr = **nunki_server_IP_address_list;
    TCP_socket_address.sin_port = htons(CLIENT_TCP_PORT_NUMBER);
    if (bind(TCP_socket_descriptor, (struct sockaddr *) &TCP_socket_address, sizeof(TCP_socket_address)) < 0) {
        display_error_message_int("Error binding to TCP socket ", TCP_socket_descriptor, BIND_TO_TCP_SOCKET_ERROR);
    }
    update_socket_info(TCP_socket_descriptor, &TCP_socket_address);
}

void listen_to_TCP_socket() {
    if (listen(TCP_socket_descriptor, 5) < 0) {
        display_error_message_int("Error listening to TCP socket ", TCP_socket_descriptor,
                                  LISTEN_TO_TCP_SOCKET_ERROR);
    }
}

void establish_TCP_connection() {
    int address_length = sizeof(server_A_TCP_socket_address);
    while ((server_A_TCP_socket_descriptor = accept(TCP_socket_descriptor,
                                                    (struct sockaddr *) &server_A_TCP_socket_address,
                                                    (socklen_t *) &address_length)) < 0) {
        if ((errno != ECHILD) && (errno != ERESTART) && (errno != EINTR)) {
            display_error_message_int("Error accepting incoming TCP connection for socket ",
                                      TCP_socket_descriptor, ACCEPT_TCP_SOCKET_ERROR);
        }
    }
    while ((server_B_TCP_socket_descriptor = accept(TCP_socket_descriptor,
                                                    (struct sockaddr *) &server_B_TCP_socket_address,
                                                    (socklen_t *) &address_length)) < 0) {
        if ((errno != ECHILD) && (errno != ERESTART) && (errno != EINTR)) {
            display_error_message_int("Error accepting incoming TCP connection for socket ",
                                      TCP_socket_descriptor, ACCEPT_TCP_SOCKET_ERROR);
        }
    }
    while ((server_C_TCP_socket_descriptor = accept(TCP_socket_descriptor,
                                                    (struct sockaddr *) &server_C_TCP_socket_address,
                                                    (socklen_t *) &address_length)) < 0) {
        if ((errno != ECHILD) && (errno != ERESTART) && (errno != EINTR)) {
            display_error_message_int("Error accepting incoming TCP connection for socket ",
                                      TCP_socket_descriptor, ACCEPT_TCP_SOCKET_ERROR);
        }
    }
    while ((server_D_TCP_socket_descriptor = accept(TCP_socket_descriptor,
                                                    (struct sockaddr *) &server_D_TCP_socket_address,
                                                    (socklen_t *) &address_length)) < 0) {
        if ((errno != ECHILD) && (errno != ERESTART) && (errno != EINTR)) {
            display_error_message_int("Error accepting incoming TCP connection for socket ",
                                      TCP_socket_descriptor, ACCEPT_TCP_SOCKET_ERROR);
        }
    }
}

void receive_neighbor_info_over_TCP() {
    char buffer[TCP_MESSAGE_LENGTH];
    while (recv(server_A_TCP_socket_descriptor, buffer, TCP_MESSAGE_LENGTH, 0) < 0) {
        if ((errno != ECHILD) && (errno != ERESTART) && (errno != EINTR)) {
            display_error_message_int("Error receiving data over child TCP socket ",
                                      server_A_TCP_socket_descriptor, READ_DATA_OVER_TCP_ERROR);
        }
    }
    add_to_server_cost(buffer);
    print_receive_info('A', &server_A_TCP_socket_address);

    while (recv(server_B_TCP_socket_descriptor, buffer, TCP_MESSAGE_LENGTH, 0) < 0) {
        if ((errno != ECHILD) && (errno != ERESTART) && (errno != EINTR)) {
            display_error_message_int("Error receiving data over child TCP socket ",
                                      server_B_TCP_socket_descriptor, READ_DATA_OVER_TCP_ERROR);
        }
    }
    add_to_server_cost(buffer);
    print_receive_info('B', &server_B_TCP_socket_address);

    while (recv(server_C_TCP_socket_descriptor, buffer, TCP_MESSAGE_LENGTH, 0) < 0) {
        if ((errno != ECHILD) && (errno != ERESTART) && (errno != EINTR)) {
            display_error_message_int("Error receiving data over child TCP socket ",
                                      server_C_TCP_socket_descriptor, READ_DATA_OVER_TCP_ERROR);
        }
    }
    add_to_server_cost(buffer);
    print_receive_info('C', &server_C_TCP_socket_address);

    while (recv(server_D_TCP_socket_descriptor, buffer, TCP_MESSAGE_LENGTH, 0) < 0) {
        if ((errno != ECHILD) && (errno != ERESTART) && (errno != EINTR)) {
            display_error_message_int("Error receiving data over child TCP socket ",
                                      server_D_TCP_socket_descriptor, READ_DATA_OVER_TCP_ERROR);
        }
    }
    add_to_server_cost(buffer);
    print_receive_info('D', &server_D_TCP_socket_address);
}

void add_to_server_cost(char *buffer) {
    int server_number = (int) buffer[40] - ASCII_A;
    char cost_string[MESSAGE_PART_LENGTH];
    int cost;
    int i, j;
    for (i = 0; i < NUM_SERVER; i++) {
        for (j = 0; j < MESSAGE_PART_LENGTH - 1; j++) {
            cost_string[j] = buffer[i * (MESSAGE_PART_LENGTH - 1) + j];
        }
        cost_string[j] = '\0';
        cost = atoi(cost_string);
        server_cost[server_number][i] = cost;
    }
}

void send_network_topology_over_UDP() {
    char buffer[UDP_MESSAGE_LENGTH];
    prepare_buffer_message(buffer);
    memset((char *) &server_UDP_socket_address, 0, sizeof(server_UDP_socket_address));
    struct hostent *server_IP_address_list_raw = resolve_host_name(HOST_NAME);
    struct in_addr **server_IP_address_list = (struct in_addr **) server_IP_address_list_raw->h_addr_list;
    server_UDP_socket_address.sin_family = AF_INET;
    server_UDP_socket_address.sin_addr = **server_IP_address_list;
    server_UDP_socket_address.sin_port = htons(SERVER_A_UDP_PORT_NUMBER);
    if (sendto(UDP_socket_descriptor, buffer, UDP_MESSAGE_LENGTH, 0, (struct sockaddr *) &server_UDP_socket_address,
               sizeof(server_UDP_socket_address)) < 0) {
        display_error_message_int("Error sending data to server A over UDP socket", UDP_socket_descriptor,
                                  SEND_DATA_OVER_UDP_ERROR);
    }
    print_send_info('A', &server_UDP_socket_address);
    server_UDP_socket_address.sin_port = htons(SERVER_B_UDP_PORT_NUMBER);
    if (sendto(UDP_socket_descriptor, buffer, UDP_MESSAGE_LENGTH, 0, (struct sockaddr *) &server_UDP_socket_address,
               sizeof(server_UDP_socket_address)) < 0) {
        display_error_message_int("Error sending data to server A over UDP socket", UDP_socket_descriptor,
                                  SEND_DATA_OVER_UDP_ERROR);
    }
    print_send_info('B', &server_UDP_socket_address);
    server_UDP_socket_address.sin_port = htons(SERVER_C_UDP_PORT_NUMBER);
    if (sendto(UDP_socket_descriptor, buffer, UDP_MESSAGE_LENGTH, 0, (struct sockaddr *) &server_UDP_socket_address,
               sizeof(server_UDP_socket_address)) < 0) {
        display_error_message_int("Error sending data to server A over UDP socket", UDP_socket_descriptor,
                                  SEND_DATA_OVER_UDP_ERROR);
    }
    print_send_info('C', &server_UDP_socket_address);
    server_UDP_socket_address.sin_port = htons(SERVER_D_UDP_PORT_NUMBER);
    if (sendto(UDP_socket_descriptor, buffer, UDP_MESSAGE_LENGTH, 0, (struct sockaddr *) &server_UDP_socket_address,
               sizeof(server_UDP_socket_address)) < 0) {
        display_error_message_int("Error sending data to server A over UDP socket", UDP_socket_descriptor,
                                  SEND_DATA_OVER_UDP_ERROR);
    }
    print_send_info('D', &server_UDP_socket_address);
}

void prepare_buffer_message(char *buffer) {
    char UDP_message_part[MESSAGE_PART_LENGTH];
    int i, j, k;
    for (i = 0; i < NUM_SERVER; i++) {
        for (j = 0; j < NUM_SERVER; j++) {
            nitoa(server_cost[i][j], UDP_message_part, 10);
            for (k = 0; k < MESSAGE_PART_LENGTH - 1; k++) {
                buffer[(i * NUM_SERVER + j) * (MESSAGE_PART_LENGTH - 1) + k] = UDP_message_part[k];
            }
        }
    }
    buffer[UDP_MESSAGE_LENGTH - 1] = '\0';
}

void calculate_network_MST() {
    int parent[NUM_SERVER];
    int key[NUM_SERVER];
    int not_included[NUM_SERVER];
    int i;
    for (i = 0; i < NUM_SERVER; i++) {
        key[i] = INT_MAX;
        not_included[i] = FALSE;
    }
    key[0] = 0;
    parent[0] = -1;
    for (i = 0; i < NUM_SERVER - 1; i++) {
        int u = min_key(key, not_included);
        int v;

        not_included[u] = TRUE;
        for (v = 0; v < NUM_SERVER; v++) {
            if (server_cost[u][v] && not_included[v] == FALSE && server_cost[u][v] < key[v]) {
                parent[v] = u, key[v] = server_cost[u][v];
            }
        }
    }
    for (i = 1; i < NUM_SERVER; i++) {
        network_MST[parent[i]][i] = server_cost[parent[i]][i];
        network_MST[i][parent[i]] = server_cost[i][parent[i]];
    }
    print_network_MST_info();
}

int min_key(int key[], int not_included[]) {
    int min = INT_MAX, min_index = 0;
    int vertex;
    for (vertex = 0; vertex < NUM_SERVER; vertex++)
        if (not_included[vertex] == FALSE && key[vertex] < min)
            min = key[vertex], min_index = vertex;
    return min_index;
}

void close_sockets() {
    if (close(TCP_socket_descriptor) < 0) {
        display_error_message_int("Error closing TCP socket ", TCP_socket_descriptor, TCP_SOCKET_CLOSE_ERROR);
    }
    if (close(server_A_TCP_socket_descriptor) < 0) {
        display_error_message_int("Error closing TCP socket ", server_A_TCP_socket_descriptor, TCP_SOCKET_CLOSE_ERROR);
    }
    if (close(server_B_TCP_socket_descriptor) < 0) {
        display_error_message_int("Error closing TCP socket ", server_B_TCP_socket_descriptor, TCP_SOCKET_CLOSE_ERROR);
    }
    if (close(server_C_TCP_socket_descriptor) < 0) {
        display_error_message_int("Error closing TCP socket ", server_C_TCP_socket_descriptor, TCP_SOCKET_CLOSE_ERROR);
    }
    if (close(server_D_TCP_socket_descriptor) < 0) {
        display_error_message_int("Error closing TCP socket ", server_D_TCP_socket_descriptor, TCP_SOCKET_CLOSE_ERROR);
    }
    if (close(UDP_socket_descriptor) < 0) {
        display_error_message_int("Error closing UDP socket ", UDP_socket_descriptor, UDP_SOCKET_CLOSE_ERROR);
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
        display_error_message_int("Error getting sockect name for socket ",
                                  socket_descriptor, GET_SOCKET_NAME_ERROR);
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

void print_receive_info(char server_name, struct sockaddr_in *server_TCP_socket_address) {
    int i;
    putchar('\n');
    printf("The Client receives neighbor information from the Server %c with TCP port number %d and IP address %s.\n",
           server_name, ntohs(server_TCP_socket_address->sin_port), inet_ntoa(server_TCP_socket_address->sin_addr));
    putchar('\n');
    printf("The Server %c has the following neighbor information:\n", server_name);
    printf("Neighbor------Cost\n");
    for (i = 0; i < NUM_SERVER; i++) {
        int cost = server_cost[server_name - ASCII_A][i];
        if (cost) {
            printf("server%c %10d\n", i + ASCII_A, cost);
        }
    }
    putchar('\n');
    printf("For this connection with Server %c, the Client has TCP port number %d and IP address %s.\n",
           server_name, ntohs(TCP_socket_address.sin_port), inet_ntoa(TCP_socket_address.sin_addr));
}

void print_send_info(char server_name, struct sockaddr_in *server_UDP_socket_address) {
    putchar('\n');
    printf("The Client has sent the network topology to the network topology to the Server "
                   "%c with UDP port number %d and IP address %s as follows:\n", server_name,
           ntohs(server_UDP_socket_address->sin_port), inet_ntoa(server_UDP_socket_address->sin_addr));
    putchar('\n');
    print_edge_cost(server_cost);
    putchar('\n');
    printf("For this connection with Server %c, the Client has UDP port number %d and IP address %s.\n",
           server_name, ntohs(UDP_socket_address.sin_port), inet_ntoa(UDP_socket_address.sin_addr));
}

void print_network_MST_info() {
    putchar('\n');
    printf("The Client has calculated a tree. The tree cost is %ld:", calculate_network_MST_cost());
    putchar('\n');
    print_edge_cost(network_MST);
}

void print_edge_cost(int edge_cost[NUM_SERVER][NUM_SERVER]) {
    int i, j;
    printf("Edge------Cost\n");
    for (i = 0; i < NUM_SERVER; i++) {
        for (j = i + 1; j < NUM_SERVER; j++) {
            int cost = edge_cost[i][j];
            if (cost) {
                printf("%c%c  %10d\n", i + ASCII_A, j + ASCII_A, cost);
            }
        }
    }
}

long calculate_network_MST_cost() {
    int i, j;
    long sum = 0;
    for (i = 0; i < NUM_SERVER; i++) {
        for (j = i + 1; j < NUM_SERVER; j++) {
            sum += network_MST[i][j];
        }
    }
    return sum;
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
