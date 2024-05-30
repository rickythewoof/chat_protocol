#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "common/const.h"
#include <signal.h>

int client_socket;

void handle_connection(int client_socket, const char *user_name) {
    char buffer[BUFFER_SIZE] = {0};
    char message[BUFFER_SIZE] = {0};

    // Send user name to server
    send(client_socket, user_name, strlen(user_name), 0);

    while (1) {
        // Get user input
        printf("Enter message: ");
        fgets(message, BUFFER_SIZE, stdin);
        message[strcspn(message, "\n")] = 0;

        // Send message to server
        send(client_socket, message, strlen(message), 0);

        // Receive message from server
        int valread = read(client_socket, buffer, BUFFER_SIZE);
        if (valread == 0) {
            printf("Server disconnected\n");
            break;
        }
        printf("Server: %s\n", buffer);
    }
}

void sigint_handler(int signum) {
    printf("\nCaught closing interrupt");
    close_connection(client_socket);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <server_ip> <user_name>\n", argv[0]);
        return 1;
    }
    struct sigaction sa = {0};
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, sa, NULL);

    const char *server_ip = argv[1];
    int server_port = SERVER_PORT;

    // Create a socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Failed to create socket");
        return 1;
    }

    // Set up server address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server_port);
    if (inet_pton(AF_INET, server_ip, &(server_address.sin_addr)) <= 0) {
        perror("Invalid server IP address");
        return 1;
    }

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("Failed to connect to server");
        return 1;
    }

    handle_connection(client_socket, argv[2]);

    // Close the socket
    close(client_socket);

    return 0;
}