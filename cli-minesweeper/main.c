#include <stdlib.h>
#include <stdio.h>
#include "data/data.h"
#include "input/input.h"

// With a lot of this same code, I can make an ascii art canvas program...

// TODO:
//  - on loss, display all unflagged mine locations
//  - arrowkey support
//  - have a way to hide cursor
//      (what if cursor is only the brackets without *, so you can see number)
//      - Cursor will be the cyan brackets. If the block is hidden/flagged, whatever
//       the cursor is covering will also be cyan.
//      - If the cell is uncovered, the cursor will display the colored number
//        in the cyan brackets, or if no number then only brackets will be shown.

/*
    NOTE:
    This project is being put in stasis
    seeing how it is functionally complete and there is nothing left for me
    here to learn. Someday I may come back and implement the final
    QoL features.

    For now, cli-minesweeper has the "Ethan Certificate of Completion"
*/

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