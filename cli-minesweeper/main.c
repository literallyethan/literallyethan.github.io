#include <stdlib.h>
#include <stdio.h>
#include "data/data.h"
#include "input/input.h"

// With a lot of this same code, I can make an ascii art canvas program...

// TODO:
//  - on loss, display all unflagged mine locations
//  - arrowkey support
//  - isolate all rendering / input in 'view' (make cross-platform with prepro)
//  - have text render centered

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
    char difficulty[2] = " ";
    
    if(argc < 2) {
        puts("Choose difficulty (easy, medium, hard)");
        difficulty[0] = getchar();
    } else {
        difficulty[0] = argv[1][0];
    }
    
    if(initialize_game(difficulty[0])) {
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