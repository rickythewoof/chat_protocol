#ifndef MESSAGE_H
#define MESSAGE_H

#include <time.h>
#include "const.h"

// Struct for a single message

typedef enum MessageType {
    CLOSE,
    CONNECT,
    MESSAGE,
    ERROR
} MessageType;

typedef struct Message {
    MessageType type;
    time_t timestamp;
    char sender[MAX_USERNAME_LENGTH];
    char content[MAX_MESSAGE_LENGTH];
    struct Message* prev;
    struct Message* next;
} Message;

// Doubly linked list for all messages
typedef struct MessageList {
    Message* head;
    Message* tail;
} MessageList;

extern Message* LastMessage;
extern char sender[MAX_USERNAME_LENGTH];

// Function to create a new message
Message* makeMessage(MessageType type, const char* content);

// Function to add a message to the message list
void sendMessage(MessageList* list, int srv_socket, Message* message);

void toString(MessageList* list);

void handle_message(Message* message);

#endif /* MESSAGE_H */