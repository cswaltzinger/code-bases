#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/sendfile.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
    #include <dirent.h>

#define BUFFER_SIZE 4096
#define MAX_PATH_LEN 256

int PORT = 8080;
char* root_dir = ".";

/**
 * @brief Sends an HTTP response back to the client.
 * @param client_socket The connected client socket.
 * @param status_code The HTTP status code (e.g., "200 OK").
 * @param content The body of the response (can be NULL).
 * @param content_type The MIME type of the content (e.g., "text/plain").
 */
void send_response(int client_socket, const char *status_code, const char *content, const char *content_type) {
    char header_buffer[512];
    int content_length = content ? strlen(content) : 0;

    // Format the HTTP headers
    snprintf(header_buffer, sizeof(header_buffer),
             "HTTP/1.1 %s\r\n"
             "Content-Type: %s\r\n"
             "Content-Length: %d\r\n"
             "Connection: close\r\n"
             "\r\n",
             status_code, content_type, content_length);

    // Send headers
    send(client_socket, header_buffer, strlen(header_buffer), 0);

    // Send content (if any)
    if (content) {
        send(client_socket, content, content_length, 0);
    }
}


void  handle_GET(int client_socket,  char* safe_filename){
    int fd = open(safe_filename, O_RDONLY);
    if (fd < 0) {
        perror("open failed");
        send_response(client_socket, "404 Not Found", "File not found.", "text/plain");
        return;
    }
    int written ;
    do{
        written = sendfile(client_socket, fd, 0, BUFFER_SIZE);

    }while(written == BUFFER_SIZE);
    close(fd);
}

/**
 * @brief Performs basic sanitization on the URI to prevent path traversal (../).
 * @param uri The raw URI from the HTTP request.
 * @param safe_path Output buffer for the sanitized path.
 * @return 0 on success (path is safe), -1 on failure (path is unsafe).
 */
int sanitize_path(const char *uri, char *safe_path) {
    // 1. Check for '..' components, which indicates path traversal attempt.
    if (strstr(uri, "..")) {
        return -1; // Unsafe
    }

    // 2. Strip leading slash and normalize the path.
    // The path will be stored in the server's current directory.
    const char *start = uri;
    if (start[0] == '/') {
        start++;
    }

    // 3. Ensure path is not too long
    size_t path_len = strlen(start);
    if (path_len == 0 || path_len >= MAX_PATH_LEN) {
        return -1; // Invalid or too long
    }

    // Copy to the safe_path buffer
    strncpy(safe_path, start, MAX_PATH_LEN - 1);
    safe_path[MAX_PATH_LEN - 1] = '\0';

    return 0; // Safe
}

/**
 * @brief Handles the incoming client request.
 * @param client_socket The connected client socket.
 */
void handle_request(int client_socket) {
    char request_buffer[BUFFER_SIZE] = {0};
    long valread;

    // Read the first chunk of data (headers + maybe start of body)
    valread = recv(client_socket, request_buffer, BUFFER_SIZE - 1, 0);
    if (valread <= 0) {
        close(client_socket);
        return;
    }
    if(0){
        printf("Received %ld bytes\n", valread);
        int i = 0 ;
        while(request_buffer[i] != '\0'){
            if(request_buffer[i] == '\r')
                printf("\\r");
            else if(request_buffer[i] == '\n')
                printf("\\n\n");
            else
                printf("%c",request_buffer[i]);
            i++;
        }
        printf("\n");
    }

    // Null-terminate the buffer
    request_buffer[valread] = '\0';

    char method[10];
    char uri[MAX_PATH_LEN * 2]; // URI can be longer than final file path
    int content_length = 0;
    char *body_start = NULL;

    // --- 1. Parse Request Line ---
    sscanf(request_buffer, "%9s %499s", method, uri);

    // Only process PUT and POST
    if (strcmp(method, "PUT") != 0 && strcmp(method, "POST") != 0 && strcmp(method, "GET") != 0) {
        send_response(client_socket, "405 Method Not Allowed", "Only PUT and POST are supported.", "text/plain");
        close(client_socket);
        return;
    }
    char safe_filename[MAX_PATH_LEN+1];
    safe_filename[0] = '/';
    if (sanitize_path(uri, safe_filename+1) != 0) {
        send_response(client_socket, "400 Bad Request", "Invalid or unsafe URI path.", "text/plain");
        close(client_socket);
        return;
    }

    if(strcmp(method, "GET") ==0){
        handle_GET(client_socket,  safe_filename);
        close(client_socket);
        return;
    }

    

    // --- 2. Parse Headers to find Content-Length and Body Start ---
    body_start = request_buffer;
    while(strncmp(body_start, "\r\n\r\n", 4) != 0 && *body_start != '\0'){
        body_start++;
    }
    // Find the end of headers (empty line: "\r\n\r\n")
    // body_start = strstr(request_buffer, "\r\n\r\n");

    if (*body_start == '\0') {
        send_response(client_socket, "400 Bad Request", "could not find start of body", "text/plain");
        // send_response(client_socket, "400 Bad Request", "Missing or malformed headers.", "text/plain");
        close(client_socket);
        return;
    }
    body_start += 4; // Move past "\r\n\r\n"
    // printf("\n\n%s\n\n",body_start);

    // Calculate how much body data was read in the initial buffer
    long body_data_read = valread - (body_start - request_buffer);

    // --- 3. Validate and Sanitize URI for Filename ---
    
    // printf("Sanitized filename: %s\n", safe_filename);

    // --- 4. Write Data to File ---
    // printf("Writing to file: %s\n", safe_filename);

    int fd = open(safe_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("open failed");
        send_response(client_socket, "500 Internal Server Error", "Could not open file for writing.", "text/plain");
        close(client_socket);
        return;
    }
    int written = write(fd, body_start, strlen(body_start));
    // Write the part of the body that was already read
    int max = 20;
    content_length =0;
    do{
        written = sendfile(fd, client_socket, 0, BUFFER_SIZE);
        // content_length += written;
    }while(written == BUFFER_SIZE && max--);
    close(fd);

    // --- 5. Send Success Response ---
    char response_msg[128];
    snprintf(response_msg, sizeof(response_msg), "Successfully stored %d bytes to file: %s", written, safe_filename);
    send_response(client_socket, "201 Created", response_msg, "text/plain");

    close(client_socket);
    printf("Handled request: %s %s %s %d\n", method, uri, safe_filename,written);
}

int main(int argc, char *argv[]) {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int opt = 1;
    

    if(argc ==2){
        root_dir = argv[1];

    }else if (argc == 3){
        root_dir = argv[1];
        PORT = atoi(argv[2]);
    }else if (argc > 3){
        fprintf(stderr, "Usage: %s [root_directory] [port]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Change working directory to root_dir
    if(chroot(root_dir) != 0){
        perror("chroot failed");
        exit(EXIT_FAILURE);
    }



    
    if(0){
        printf("root directory changed to: %s\n", root_dir);
        printf("Files in root directory:\n");
        DIR *dir = opendir("/");
        if (dir) {
            struct dirent *entry;
            while ((entry = readdir(dir)) != NULL) {
                if (entry->d_name[0] != '.') { // Skip hidden files and . ..
                    printf("  %s\n", entry->d_name);
                }
            }
            closedir(dir);
        } else {
            perror("opendir failed");
        }
    }
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully set socket options to reuse address/port
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // Listen on all interfaces
    address.sin_port = htons(PORT);

    // Bind the socket to the specified port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 10) < 0) {
        perror("listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("--- Simple C HTTP File Server ---\n");
    printf("Listening on port %d\n", PORT);
    printf("Files will be stored in the root directory.\n\n");

    // Main loop to accept and handle connections
    while (1) {
        // printf("Waiting for a connection...\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            // perror("accept failed");
            continue; // Keep listening
        }

        handle_request(new_socket);
    }

    // Should not be reached, but good practice
    close(server_fd);
    return 0;
}
