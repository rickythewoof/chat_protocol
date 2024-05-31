#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "common/const.h"
#include "common/message.h"
#include <signal.h>

int client_socket;
MessageList list;

void handle_message(Message* message){
    switch (message->type){
        case(CLOSE):
            printf("User %s disconnected\n", message->sender);
            break;
        case(CONNECT):
            printf("User %s connected\n", message->sender);
            break;
        case(MESSAGE):
            printf("@%s: %s\n", message->sender, message->content);
            break;
    }
}

void close_connection(int client_socket) {
    Message* close_message = makeMessage(CLOSE, sender, "", "");
    sendMessage(&list, client_socket, close_message);
    close(client_socket);
}

void handle_connection(int client_socket, const char *user_name) {
    // Send user name to server
    Message* connect_message = makeMessage(CONNECT, user_name, "", "");
    send(client_socket, connect_message, sizeof(Message), 0);
    while (1) {
        // Get user input
        char contents[MAX_MESSAGE_LENGTH];
        printf("Enter message: ");
        fgets(contents, MAX_MESSAGE_LENGTH, stdin);
        contents[strcspn(contents, "\n")] = 0;
        message = makeMessage(MESSAGE, sender, contents)
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
    if(strlen(argv[2]) > MAX_USERNAME_LENGTH){
        printf("Username too long\n");
        return 1;
    }
    strncpy(sender, argv[2], MAX_USERNAME_LENGTH);
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