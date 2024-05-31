#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/select.h>
#include <errno.h>
#include "common/const.h"
#include "common/message.h"

int client_sockets[MAX_CLIENTS];

void* message_handler(void* arg){
    fd_set read_fds;
    FD_ZERO(&read_fds);
    int max_fd;
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_sockets[i] > max_fd) {
            FD_SET(client_sockets[i], &read_fds);
            max_fd = client_sockets[i];
        }
    }

    int act = select(max_fd + 1, &read_fds, NULL, NULL, NULL);
    if (act < 0) {
        perror("select error");
        return 1;
    }
    if (FD_ISSET(client_sockets[0], &read_fds)) {
        Message* buffer = {0};
        read(client_sockets[0], buffer, BUFFER_SIZE);
        switch (buffer->type){
            case(CLOSE):
                printf("User %s disconnected\n", buffer->sender);
                close(client_sockets[0]);
                break;
            case(CONNECT):
                printf("User %s connected\n", buffer->sender);
                break;
            case(MESSAGE):
                printf("User %s sent message %s\n", buffer->sender, buffer->content);
                break;

        }

        for(int j = 0; j < MAX_CLIENTS; j++) {
            if (client_sockets[j] != 0 && client_sockets[j] != client_sockets[0]) {
                send(client_sockets[j], buffer, strlen(buffer), 0);
            }
        }
    }
}

int main(int argc, char** argv) {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    
    // Create server socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    // Set server address and port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = SERVER_PORT;
    
    // Bind the socket to the specified address and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    
    // Listen for incoming connections
    if (listen(server_fd, MAX_CLIENTS) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }
    
    printf("Server listening on port %d\n", SERVER_PORT);
    
    // Initialize client sockets array
    int i;
    for (i = 0; i < MAX_CLIENTS; i++) {
        client_sockets[i] = 0;
    }
    pthread_t message_thread;
    pthread_create(&message_thread, NULL, message_handler, NULL);
    pthread_detach(message_thread);

    while (1) {
        // Accept incoming connection
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept failed");
            exit(EXIT_FAILURE);
        }
        
        // Add new socket to client sockets array
        for (i = 0; i < MAX_CLIENTS; i++) {
            if (client_sockets[i] == 0) {
                client_sockets[i] = new_socket;
                break;
            }
        }
    }  
    return 0;
}