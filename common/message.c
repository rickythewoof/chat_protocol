#include <stdio.h>
#include <string.h>

#include "message.h"

Message* LastMessage;

// Function to create a new message
Message* createMessage(int id, const char* sender, const char* receiver, const char* content) {
    Message* message = (Message*)malloc(sizeof(Message));
    message->timestamp = time(NULL);
    message->sender = sender;
    message->receiver = receiver;
    message->content = content;
    message->prev = LastMessage;
    LastMessage = message;
    message->next = NULL;
    return message;
}
