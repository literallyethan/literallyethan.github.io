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

SECTION 2.
    MODULES

    

SECTION 3.
    DATA FLOW

    Each module of the game is single-responsibility,
    that is, it only does one thing.

    If you can't decide if something should go in one module
    or another, it should be its own module.

