#include "server.h"

void handle_request(int new_socket, char *buffer) {
    // (Modified to be more generic)
    char method[16], path[256], protocol[16];
    sscanf(buffer, "%s %s %s", method, path, protocol);

    char response[BUFFER_SIZE];
    sprintf(response, "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 30\n\nResponse from Backend: %s\n", path);

    write(new_socket, response, strlen(response));
    printf("Request handled for path: %s\n", path);
    log_message("Request handled by backend");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    int port = atoi(argv[1]);

    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        error("socket failed");
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        error("bind failed");
    }

    if (listen(server_fd, 3) < 0) {
        error("listen");
    }

    printf("Backend server listening on port %d\n", port);
    log_message("Backend server started");

    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            error("accept");
        }

        read(new_socket, buffer, BUFFER_SIZE);
        handle_request(new_socket, buffer);
        close(new_socket);
    }

    return 0;
}
