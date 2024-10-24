#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <termios.h>
#include "input.h"

static struct termios original_termios;

// Signal handler to catch SIGINT and SIGSEGV
void handle_signal(int sig) {
    if (sig == SIGSEGV) {
        printf("Segmentation fault caught!\n");
    } else if (sig == SIGINT) {
        printf("Ctrl+C caught!\n");
    }

    restore_input_mode(); // Restore terminal settings
    puts("Phew...");
    exit(1);  // Exit after handling the signal
}

// Function to disable terminal echoing and line buffering
void set_input_mode() {
    struct termios new_termios;

    // Save the current terminal settings
    tcgetattr(STDIN_FILENO, &original_termios);

    // Register restore_input_mode to be called when the program exits
    atexit(restore_input_mode);

    // Set signal handlers for SIGINT and SIGSEGV
    signal(SIGINT, handle_signal);
    signal(SIGSEGV, handle_signal);

    // Copy the original settings to modify them
    new_termios = original_termios;

    // Disable canonical mode and echo
    new_termios.c_lflag &= ~(ICANON | ECHO);

    // Apply the new settings immediately
    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
}

// Function to restore the original terminal settings
void restore_input_mode() {
    // Restore the original terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &original_termios);
}

char poll_input() {
    char input = getchar(); fflush(stdout);
    return input;
}