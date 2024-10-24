#ifndef INPUT_H
#define INPUT_H

#include <stdio.h>
#include <termios.h>
#include <unistd.h>

// allow for proper input handling
void set_input_mode();

// resets the terminal mode to the default
void restore_input_mode();

// in case of emergency, restore terminal
void handle_signal(int sig);

char poll_input();

#endif