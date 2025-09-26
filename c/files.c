#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int main() {
    // Open a file for reading. Replace "source.txt" with your file's name.
    int source_fd = open("source.txt", O_RDONLY);
    if (source_fd < 0) {
        perror("Failed to open source file");
        return 1;
    }

    // Create or open a destination file for writing.
    // O_CREAT creates the file if it doesn't exist.
    // O_WRONLY opens it for writing.
    // The mode 0b111100100 gives rwx-r--r-- permissions 
    int dest_fd = open("destination.txt", O_CREAT | O_WRONLY, 0b111100100);
    if (dest_fd < 0) {
        perror("Failed to open destination file");
        close(source_fd);
        return 1;
    }

    char buffer[1024];
    ssize_t bytes_read;

    // Loop to read from the source file in chunks and write to the destination.
    // read() returns the number of bytes read, or 0 at end of file, or -1 on error.
    while ((bytes_read = read(source_fd, buffer, sizeof(buffer))) > 0) {
        // Write the same number of bytes that were just read.
        if (write(dest_fd, buffer, bytes_read) != bytes_read) {
            perror("Failed to write to destination file");
            close(source_fd);
            close(dest_fd);
            return 1;
        }
    }

    if (bytes_read < 0) {
        perror("Error while reading source file");
    }

    // Close both file descriptors.
    close(source_fd);
    close(dest_fd);

    printf("File copied successfully.\n");
    return 0;
}
