#ifndef MESSAGE_H
#define MESSAGE_H

#include <time.h>

// Struct for a single message
typedef struct Message {
    time_t timestamp;
    const char* sender;
    const char* receiver;
    const char* content;
    struct Message* prev;
    struct Message* next;
} Message;

// Doubly linked list for all messages
typedef struct MessageList {
    Message* head;
    Message* tail;
} MessageList;

// Function to create a new message
Message* createMessage(int id, const char* sender, const char* receiver, const char* content);

// Function to add a message to the message list
void addMessage(MessageList* list, Message* message);

// Function to remove a message from the message list
void removeMessage(MessageList* list, Message* message);

// Function to print all messages in the message list
void printMessages(MessageList* list);

void LoadMessages(MessageList* list);

#endif /* MESSAGE_H */