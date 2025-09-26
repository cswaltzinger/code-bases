#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define BUFFER_SIZE 1024
int main() {
    FILE *source_file = NULL;
    FILE *dest_file = NULL;
    
    // Buffer to hold data during transfer
    char buffer[BUFFER_SIZE];
    size_t bytes_read;

    // 1. Open source file for reading in binary mode ("rb")
    source_file = fopen(source_path, "rb");
    if (source_file == NULL) {
        perror("Error opening source file for reading");
        return 1;
    }

    // 2. Open destination file for writing in binary mode ("wb")
    // If the file exists, "wb" truncates it; if it doesn't exist, it creates it.
    dest_file = fopen(dest_path, "wb");
    if (dest_file == NULL) {
        perror("Error opening destination file for writing");
        // Ensure source file is closed before exiting
        fclose(source_file);
        return 1;
    }

    // 3. Read loop: Read chunks from source and write them to destination
    // fread returns the number of items (size_t) successfully read.
    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, source_file)) > 0) {
        // Write the same number of bytes that were just read.
        // fwrite returns the number of items successfully written.
        size_t bytes_written = fwrite(buffer, 1, bytes_read, dest_file);
        
        if (bytes_written != bytes_read) {
            fprintf(stderr, "Error: Could not write all data to destination file.\n");
            // Check stream error status
            if (ferror(dest_file)) {
                perror("Write error occurred");
            }
            // Clean up and exit
            fclose(source_file);
            fclose(dest_file);
            return 1;
        }
    }

    // 4. Check for read errors (other than EOF)
    // ferror() returns non-zero if an error occurred on the stream.
    if (ferror(source_file)) {
        perror("Error occurred while reading source file");
        // Clean up and exit
        fclose(source_file);
        fclose(dest_file);
        return 1;
    }
    
    // 5. Cleanup: Close both file streams
    fclose(source_file);
    fclose(dest_file);

    printf("File copied successfully.\n");
    return 0;
}
