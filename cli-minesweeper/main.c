#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include "data/data.h"
#include "input/input.h"

// With a lot of this same code, I can make an ascii art canvas program...

void usage() {
    puts("Usage: ");
    puts("\t./minesweeper [easy/medium/hard]\n");
    
    //puts("\t./minesweeper custom [w] [h] [mine count]\n");
}

void game_over() {
    puts("YOU LOSE!!!");
}

void game_win() {
    puts("YOU WIN!!!");
}

// try using main.c as the controller
// we will forgo printing coordinates, and 
// we will use termios.h to handle button events

int main(int argc, char* argv[]) {
    if(argc < 2) {
        usage();
        return 1;
    }
    
    if(initialize_game(argv[1])) {
        puts("Initialization failed.");
        return 1;
    }

    set_input_mode();

    int status = 0;
    while (!status) {
        system("clear");
        puts(serialize_board());
        puts("------------------------");
        status = action_cursor(poll_input());
        // update game change data based on input
    }

    if(status == -1) {
        game_over();
    } else {
        game_win();
    }

    restore_input_mode();
    close_game();
    return 0;
}