#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    const char *hello = "Hello from server";

    // Create the server socket file descriptor.
    // AF_INET for IPv4, SOCK_STREAM for TCP.
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options to reuse the address and port.
    // This helps avoid "Address already in use" errors.
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // Configure the server address.
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // Listen on all available network interfaces.
    address.sin_port = htons(8080); // Port 8080.

    // Bind the socket to the specified IP address and port.
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming client connections.
    // The second argument is the maximum number of pending connections.
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    printf("Server listening on port 8080...\n");

    // Accept a client connection. This call blocks until a client connects.
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    printf("Client connected.\n");

    // Read data sent by the client.
    int valread = read(new_socket, buffer, 1024);
    printf("Client message: %s\n", buffer);

    // Send a message back to the client.
    send(new_socket, hello, strlen(hello), 0);
    printf("Message sent to client.\n");

    // Close the sockets.
    close(new_socket);
    close(server_fd);

    return 0;
}
