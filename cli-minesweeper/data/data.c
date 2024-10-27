#include "data.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>


enum cell_state {
    CELL_EMPTY,
    CELL_MINE,
    CELL_UNKNOWN,
} cell_state;

struct cell {
    int covered; // 1 = covered, 0 = revealed
    int has_mine; // 1 = yes, 0 = no
    int flagged; // 1 = yes, 0 = no
    int mines_touching;

    // when initializing board
    // if neighbor cell is within bounds
    // store neighbor cell, else store NULL
    // in order of
    /*
       [0][1][2]
       [3][X][4]
       [5][6][7]
    */
    // where X is this cell.
    struct cell* cell_refs[8]; // references to 8 nearby cells
};

struct Cursor {
    int cursorY; // accessed like y, x
    int cursorX;
};

struct DATA {
    int width;
    int height;
    int mine_count;
    int mines_flagged;
    int flags_left;
    struct cell** board; 
};

static struct DATA* GAME_DATA = NULL;

static char* BOARD_STRING = NULL;

static struct Cursor cursor;

static const char *block = "\xE2\x96\x92";  // UTF-8 encoded "▒" (U+2592)

int check_win() {
    if(GAME_DATA->mines_flagged == GAME_DATA->mine_count) {
        return 1;
    }
    return 0;
}

void populate_refs(int y, int x) {
    //ternary operations:
    //  (condition) ? true : false
    struct cell* current_cell = &(GAME_DATA->board[y][x]);

    current_cell->cell_refs[0] = (y - 1 >= 0 && x  - 1 >= 0) 
        ? &(GAME_DATA->board[y - 1][x - 1]) : NULL;

    current_cell->cell_refs[1] = (y - 1 >= 0 && x >= 0) 
        ? &(GAME_DATA->board[y - 1][x]) : NULL;

    current_cell->cell_refs[2] = (y - 1 >= 0 && x + 1 < GAME_DATA->width) 
        ? &(GAME_DATA->board[y - 1][x + 1]) : NULL;

    current_cell->cell_refs[3] = (x - 1 >= 0) 
        ? &(GAME_DATA->board[y][x - 1]) : NULL;

    current_cell->cell_refs[4] = (x + 1 < GAME_DATA->width) 
        ? &(GAME_DATA->board[y][x + 1]) : NULL;

    current_cell->cell_refs[5] = (y + 1 < GAME_DATA->height && x - 1 >= 0) 
        ? &(GAME_DATA->board[y + 1][x - 1]) : NULL;

    current_cell->cell_refs[6] = (y + 1 < GAME_DATA->height) 
        ? &(GAME_DATA->board[y + 1][x]) : NULL;

    current_cell->cell_refs[7] = (y + 1 < GAME_DATA->height && x + 1 < GAME_DATA->width) 
        ? &(GAME_DATA->board[y + 1][x + 1]) : NULL;
}

int count_touching_mines(int y, int x) {
    // cannot be called on a space with a mine,
    // assume starting space does not have one

    struct cell* current_cell = &(GAME_DATA->board[y][x]);
    int touching = 0;
    for(int i = 0; i < 8; ++i) {
        if(current_cell->cell_refs[i] == NULL) {
            continue;
        }

        if(current_cell->cell_refs[i]->has_mine == 1) {
            ++touching;
        }
    }

    current_cell->mines_touching = touching;
    return touching;
}

void clear_space(struct cell* current_cell) {

    // if the cell is already uncovered, return
    if (!current_cell->covered) {
        return;
    }

    // uncover cell
    current_cell->covered = 0;

    // if the current cell has no mines touching, continue to uncover neighbors
    if (current_cell->mines_touching == 0) {
        for (int i = 0; i < 8; ++i) {
            if (current_cell->cell_refs[i] != NULL && current_cell->cell_refs[i]->covered) {
                // recursive call
                clear_space(current_cell->cell_refs[i]);
            }
        }
    }
}

void populate_mines() {
    
    int mines_left = GAME_DATA->mine_count;
    int height = GAME_DATA->height;
    int width = GAME_DATA->width;
    int iterations = 0;
    int rand_limit = 10;
    srand(time(NULL));

    // traverse and place mines
    while(mines_left > 0) {
        for(int i = 0; i < height; ++i) {
            for(int j = 0; j < width; ++j) {
                int value = rand() % (rand_limit + 1);
                if(mines_left > 0 && value == rand_limit - 1) {
                    GAME_DATA->board[i][j].has_mine = 1;
                    mines_left -= 1;
                }
            }
        }
        ++iterations;
    }

}

int initialize_game(char mode) {
    GAME_DATA = malloc(sizeof(struct DATA));
    if (GAME_DATA == NULL) {
        puts("Data failed to initialize.");
        return 1;
    }

    switch(mode) {
        case 'e':
            GAME_DATA->width = 10;
            GAME_DATA->height = 8;
            GAME_DATA->mine_count = 10;
            break;
        case 'm':
            GAME_DATA->width = 18;
            GAME_DATA->height = 14;
            GAME_DATA->mine_count = 40;
            break;
        case 'h':
            GAME_DATA->width = 24;
            GAME_DATA->height = 20;
            GAME_DATA->mine_count = 99;
            break;
        default:
            puts("Invalid mode.");
            free(GAME_DATA);
            GAME_DATA = NULL;
            return 1;
    }
    GAME_DATA->mines_flagged = 0;
    GAME_DATA->flags_left = GAME_DATA->mine_count;

    GAME_DATA->board = malloc(sizeof(struct cell*) * (GAME_DATA->height));
    if (GAME_DATA->board == NULL) {
        puts("Board failed to initialize.");
        free(GAME_DATA);
        GAME_DATA = NULL;
        return 1;
    }

    for(int i = 0; i < GAME_DATA->height; ++i) {
        GAME_DATA->board[i] = malloc(sizeof(struct cell) * (GAME_DATA->width));
        if (GAME_DATA->board[i] == NULL) {
            puts("Row failed to initialize.");
            
            for (int k = 0; k < i; ++k) {
                free(GAME_DATA->board[k]);
            }
            free(GAME_DATA->board);  
            free(GAME_DATA);
            return 1;
        }

        for(int j = 0; j < GAME_DATA->width; ++j) {
            GAME_DATA->board[i][j].covered = 1;
            GAME_DATA->board[i][j].has_mine = 0;
        }
    }

    populate_mines();
    
    // Populate cell_refs
    for(int i = 0; i < GAME_DATA->height; ++i) {
        for (int j = 0; j < GAME_DATA->width; ++j) {
            populate_refs(i, j);
            count_touching_mines(i, j);
        }
    }

    cursor.cursorX = (GAME_DATA->width) / 2;
    cursor.cursorY = (GAME_DATA->height) / 2;

    return 0;
}

void close_game() {
    for(int i = 0; i < GAME_DATA->height; ++i) {
        free(GAME_DATA->board[i]);
        GAME_DATA->board[i] = NULL;
    }

    if (BOARD_STRING != NULL) {
        free(BOARD_STRING);
        BOARD_STRING = NULL;
    }

    free(GAME_DATA->board);
    GAME_DATA->board = NULL;
    free(GAME_DATA);
    GAME_DATA = NULL;
}

char* serialize_board() {
    if(GAME_DATA->board == NULL) {
        puts("Must initialize data first.");
        return NULL;
    }

    // This way we dont realloc the board every update
    if(BOARD_STRING == NULL) {
        BOARD_STRING = malloc(
            (GAME_DATA->height * GAME_DATA->width * 14) +  // Worst case: 12 characters per cell with ANSI codes
            (GAME_DATA->width * 3 + 2) +                  // Column labels (3 chars per label)
            (GAME_DATA->height * 2) +                     // Row labels (assuming single char per label)
            (GAME_DATA->height + 1) +                     // Newline characters after each row
            1                                            // Null terminator
        );
    }

    BOARD_STRING[0] = '\0';

    int offset = 0;  // Track where in the string we are writing
    // loop for drawing each row proper
    for(int i = 0; i < GAME_DATA->height; ++i) {
        //strcat(BOARD_STRING, " ");
        //offset += 2;
        for (int j = 0; j < (GAME_DATA->width); ++j) {
            if(cursor.cursorY == i && cursor.cursorX == j) {
                offset += sprintf(BOARD_STRING + offset, "\033[1;36m[*]\033[0m");
            } else if(GAME_DATA->board[i][j].flagged == 1) {
                offset += sprintf(BOARD_STRING + offset, "[\033[35m!\033[0m]");
            } /*else if(GAME_DATA->board[i][j].has_mine == 1) {
                offset += sprintf(BOARD_STRING + offset, "[\033[31m@\033[0m]");
            }*/ else if(GAME_DATA->board[i][j].covered == 1) {
                char str[] = {'[', '#'/*(char)178*/, ']', '\0'};
                
                offset += sprintf(BOARD_STRING + offset, "%s", str);
            } else if(GAME_DATA->board[i][j].mines_touching != 0) {
                char num = GAME_DATA->board[i][j].mines_touching + '0';
                char str[20]; 
                switch(num) {
                    case '1':
                        strcpy(str, " \033[36m \033[0m ");
                        str[6] = num;
                        break;
                    case '2':
                        strcpy(str, " \033[32m \033[0m ");
                        str[6] = num;
                        break;
                    case '3':
                        strcpy(str, " \033[31m \033[0m "); 
                        str[6] = num;
                        break;
                    case '4':
                        strcpy(str, " \033[33m \033[0m "); 
                        str[6] = num;
                        break;
                    case '5':
                        strcpy(str, " \033[34m \033[0m "); 
                        str[6] = num;
                        break;
                    case '6':
                        strcpy(str, " \033[34m \033[0m "); 
                        str[6] = num;
                        break;
                    case '7':
                        strcpy(str, " \033[34m \033[0m ");
                        str[6] = num;
                        break;
                    case '8':
                        strcpy(str, " \033[34m \033[0m "); 
                        str[6] = num;
                        break;
                }

                offset += sprintf(BOARD_STRING + offset, "%s", str);
            } else {
                offset += sprintf(BOARD_STRING + offset, "   ");
            } 
        }
        offset += sprintf(BOARD_STRING + offset, "\n");
    }

    BOARD_STRING[strlen(BOARD_STRING)] = '\0';

    printf("Serialized board length: %zu\n", strlen(BOARD_STRING));
    printf("Flags left: %d\n", GAME_DATA->flags_left);

    return BOARD_STRING;
}

int action_cursor(char action) {
    struct cell* current_cell = &(GAME_DATA->board[cursor.cursorY][cursor.cursorX]);

    switch(action) {
        case 'w':
            if(cursor.cursorY != 0) {
                cursor.cursorY -= 1;
            }
            break;
        case 'a':
            if(cursor.cursorX != 0) {
                cursor.cursorX -= 1;
            }
            break;
        case 's':
            if(cursor.cursorY != GAME_DATA->height - 1) {
                cursor.cursorY += 1;
            }
            break;
        case 'd':
            if(cursor.cursorX != GAME_DATA->width - 1) {
                cursor.cursorX += 1;
            }
            break;
        case 'f':
            // when you flag

            if(current_cell->covered == 0) {
                break;
            }

            if(current_cell->flagged == 1) {
                current_cell->flagged = 0;
                GAME_DATA->flags_left += 1;
                if(current_cell->has_mine) {
                    GAME_DATA->mines_flagged -= 1;
                }
            } else if(GAME_DATA->flags_left > 0) {
                current_cell->flagged = 1;
                GAME_DATA->flags_left -= 1;
                if(current_cell->has_mine) {
                    GAME_DATA->mines_flagged += 1;
                    if(check_win() == 1) {
                        return 1;
                    }
                }
            }
            
            break;
        case ' ':
            if(current_cell->flagged == 0) {
                if(current_cell->has_mine) {
                    return -1;
                } else {
                    clear_space(current_cell);
                }
            }
            
            break;
        default:
            puts("Invalid action.");
    }
    return 0;
}
