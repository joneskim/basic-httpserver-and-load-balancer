#include "server.h"

#define PORT 8080
#define MAX_BACKENDS 3

char *backend_servers[MAX_BACKENDS] = {
    "127.0.0.1:8081",
    "127.0.0.1:8082",
    "127.0.0.1:8083"
};

int current_backend = 0;

int connect_to_backend() {
    // (This function remains the same as before)
    int backend_socket;
    struct sockaddr_in backend_addr;
    char *server_ip;
    int server_port;

    // Try connecting to backends in a loop
    for (int i = 0; i < MAX_BACKENDS; i++) {
        char backend_address[20];
        strcpy(backend_address, backend_servers[current_backend]);
        server_ip = strtok(backend_address, ":");
        if (server_ip == NULL) {
            char log_msg[128];
            sprintf(log_msg, "Invalid backend address format: %s", backend_servers[current_backend]);
            log_message(log_msg);
            continue;
        }

        char *server_port_str = strtok(NULL, ":");
        if (server_port_str == NULL) {
            char log_msg[128];
            sprintf(log_msg, "Invalid backend address format (missing port): %s", backend_servers[current_backend]);
            log_message(log_msg);
            continue;
        }
        server_port = atoi(server_port_str);
        current_backend = (current_backend + 1) % MAX_BACKENDS;

        if ((backend_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            char log_msg[128];
            sprintf(log_msg, "Failed to create socket for backend %s:%d", server_ip, server_port);
            log_message(log_msg);
            continue;
        }

        backend_addr.sin_family = AF_INET;
        backend_addr.sin_port = htons(server_port);

        if (inet_pton(AF_INET, server_ip, &backend_addr.sin_addr) <= 0) {
            char log_msg[128];
            sprintf(log_msg, "Invalid address for backend %s:%d", server_ip, server_port);
            log_message(log_msg);
            close(backend_socket);
            continue;
        }

        if (connect(backend_socket, (struct sockaddr *)&backend_addr, sizeof(backend_addr)) < 0) {
            char log_msg[128];
            sprintf(log_msg, "Failed to connect to backend %s:%d", server_ip, server_port);
            log_message(log_msg);
            close(backend_socket);
            continue;
        }

        // Successfully connected
        return backend_socket;
    }

    // All backends failed
    log_message("ERROR: All backend servers failed to connect.");
    return -1;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    int port = atoi(argv[1]);

    int server_fd, new_socket, backend_socket;
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

    log_message("Load balancer started");

    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            error("accept");
        }

        backend_socket = connect_to_backend();
        if (backend_socket == -1) {
            // Handle the case where all backends are down
            char *error_response = "HTTP/1.1 503 Service Unavailable\r\nContent-Length: 23\r\n\r\nService Unavailable\r\n";
            write(new_socket, error_response, strlen(error_response));
            close(new_socket);
            continue;
        }

        read(new_socket, buffer, BUFFER_SIZE);

        // Forward request to backend
        write(backend_socket, buffer, strlen(buffer));

        // Read response from backend
        memset(buffer, 0, BUFFER_SIZE);
        read(backend_socket, buffer, BUFFER_SIZE);

        // Forward response to client
        write(new_socket, buffer, strlen(buffer));

        close(new_socket);
        close(backend_socket);
    }

    return 0;
}
