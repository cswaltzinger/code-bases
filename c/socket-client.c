#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char *hello = "Hello from client";
    char buffer[1024] = {0};

    // Create the client socket file descriptor.
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return -1;
    }

    // Configure the server address to connect to.
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080); // Server's port.

    // Convert IPv4 addresses from text to binary form.
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        return -1;
    }

    // Connect to the server.
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection Failed");
        return -1;
    }
    printf("Connected to server.\n");

    // Send a message to the server.
    send(sock, hello, strlen(hello), 0);
    printf("Message sent to server.\n");

    // Read the response from the server.
    int valread = read(sock, buffer, 1024);
    printf("Server response: %s\n", buffer);

    // Close the socket.
    close(sock);

    return 0;
}
