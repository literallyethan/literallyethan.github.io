# literallyethan.github.io
https://literallyethan.github.io/index.html

This is my portfolio website!

Here is where I will post the projects that I am excited to
show off.

Project 1: Minesweeper
    A fun project that I made myself based on the old Microsoft game as a lesson in
    developing games without an engine, and just to excersice
    software development in general. 

    I wanted to limit the game as a CLI experience to challenge myself
    to make a game that looked good and was fun to play in such an environment.
    I learned a lot and want to keep going to see how far I can push the boundary
    of what makes a CLI game.

    One thing I know I could have done better was to make the code for the game
    more modular and reusable. The input module is pretty independent, but the data
    module is a weird amalgamation of game logic and visual display.
    In the future I will work to separate those two functions for other projects.

    To run, enter the project directory in terminal and type `make`.
    After you can either run the executable by clicking on the icon in your file manager
    or by running `./minesweeper` or `./minesweeper [easy/medium/hard]` in terminal.
    GAME ONLY WORKS ON MAC/LINUX!!! (For now.)

    CONTROLS:
        [W/A/S/D] for moving the cursor
        [F] to flag the tile the cursor hovers over
        [SPACE] for uncovering the cursor'd tile.
        [CTRL+C] ([CMD+.] for mac) for exiting the game.

    KNOWN BUGS:
        - Sometimes the amount of flags remaining can desync from
          the amount of flags places, meaning a player can flag all of the mines
          and get softlocked.


Project 2: Game Engine (long-term)
    This is a long-term project to help me understand the mechanics of
    game engines and how they work under the hood. Being such a large
    and complicated project, I expect it to teach me good practice about making
    large and complicated software projects.

    Because of the long-term nature of this project, expect it to
    be in development on and off and to change drastically in between versions.

Project 3: bibleverse
    Project that prints a random Bible verse to the terminal.
