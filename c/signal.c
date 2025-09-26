#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

// This volatile sig_atomic_t variable is used to safely communicate
// the signal event from the signal handler to the main loop.
volatile sig_atomic_t keep_running = 1;

// The signal handler function.
// It sets the flag to 0 to signal the main loop to stop.
void signal_handler(int signum) {
    printf("\nSignal %d received\n", signum);
    if(signum == 32){
        keep_running = 0;
    }
}

int main() {
    if (signal(SIGINT, signal_handler) == SIG_ERR) {
        perror("signal registration failed");
        return 1;
    }
    if (signal(32, signal_handler) == SIG_ERR) {
        perror("signal registration failed");
        return 1;
    }

    printf("Program running. Press Ctrl+C to stop.\n");

    // The main loop that continues as long as keep_running is 1.
    while (keep_running) {
        printf("Running...\n");
        sleep(1); // Pauses for 1 second.
    }

    printf("Program finished.\n");
    return 0;
}
