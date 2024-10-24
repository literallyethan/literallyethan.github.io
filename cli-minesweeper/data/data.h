#ifndef DATA_H
#define DATA_H

// creates starting data to draw game off of
int initialize_game(char* mode);

void close_game();

// Returns the board state as a string representation to be printed.
// Must be freed after use.
char* serialize_board();

// depending on the char `action`, the cursor will move in 
// a specified direction (eg. up from 'w') or it will uncover
// a cell with ' '. Can even flag with 'f'.
// returns -1 if lose, returns 1 if win.
int action_cursor(char action);

// when 
void reveal_cells();

#endif