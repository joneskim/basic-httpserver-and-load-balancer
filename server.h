#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

#define BUFFER_SIZE 1024
#define LOG_FILE "server.log"

// Function prototypes (declarations)
void log_message(const char *message);
void error(const char *msg); // Declare the error function here

#endif