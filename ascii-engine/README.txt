SECTION 1.
    HOW A GAME WORKS

    Fundamentally, a game works as a loop:

    do {
        draw_screen();
        read_input();
        update_game();
    } while (game_running);

    There are at least 3 "modules" in the game that handle 
    each individual responsibility:

    - Rendering what the player sees
    - Reading input from the player
    - Updating the game's data/logic from the new input

    Of course, the loop displayed above is an abstraction
    to show the general order of operations.

    