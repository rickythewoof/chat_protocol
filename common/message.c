#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "message.h"


// Function to create a new message
Message* makeMessage(MessageType type, const char* content){
    Message* message = (Message*)malloc(sizeof(Message));
    message->type = type;
    strncpy(message->sender, sender, MAX_USERNAME_LENGTH);
    if(type == MESSAGE){
        strncpy(message->content, content, MAX_MESSAGE_LENGTH);
    }

    return message;
}

void sendMessage(MessageList* list, int srv_socket, Message* message){
    message->timestamp = time(NULL);
    message->prev = list->tail;
    list->tail = message;
    message->next = NULL;
    int sent = send(srv_socket, message, sizeof(Message), 0);
    if(sent < 0){
        perror("Error sending message");
        exit(1);
    }
}

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

void LoadMessages(MessageList* list){
    printf("not implemented yet");
    exit(1);
}