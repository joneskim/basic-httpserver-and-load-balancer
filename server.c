#include "server.h"

// Function to log messages to a file
void log_message(const char *message) {
    FILE *log_file = fopen(LOG_FILE, "a");
    if (log_file == NULL) {
        perror("Failed to open log file");
        return;
    }

    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    char timestamp[25];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info);

    fprintf(log_file, "[%s] %s\n", timestamp, message);
    fclose(log_file);
}

// Define the error function here
void error(const char *msg) {
    char error_message[256];
    sprintf(error_message, "ERROR: %s", msg);
    perror(error_message);
    log_message(error_message);
    exit(1);
}
