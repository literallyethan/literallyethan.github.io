#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

#define VERSE_COUNT 31102
#define VERSE_SIZE 500
#define TRUE 1
#define FALSE 0

int seek_verse(char verse[], int line);
int generate_random();

/* Read a verse from bible.txt randomly and print it. */
int main(void)
{
    uint16_t random;
    char verse_buf[VERSE_SIZE];

    /* Choose some verse at random */
    random = generate_random();
    if(!seek_verse(verse_buf, random))
    {
        puts("It's the Bible, you get credit for trying.");
        exit(-1);
    }

    return(0);
}

/* Place some verse at `line` in `verse` */
int seek_verse(char verse[], int line)
{
    FILE* bible;

    bible = fopen("bible.txt", "r");

    if (bible == NULL) 
    {
        puts("May God help us all.");
        return FALSE;
    }

    

    fclose(bible);
    return(TRUE);
}

/* Generate number from 1 to VERSE_COUNT (naively seeded) */
int generate_random()
{
    uint16_t random;
    uint16_t lower_bound = 1;

    srand(time(NULL));

    /* Grab random verse line (range safety cuz im paranoid) */
try_again:
    random = rand() % (VERSE_COUNT - lower_bound + 1) + lower_bound; 
    if (random <= 0 || random > VERSE_COUNT) goto try_again;

    return(random);
}