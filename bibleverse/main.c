#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

#define VERSE_COUNT 31104
#define VERSE_SIZE 2000
#define TRUE 1
#define FALSE 0

size_t seek_verse(char verse[], size_t line);
size_t get_line(char extern_buf[], size_t len, FILE *stream);
size_t generate_random(void);

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

    puts(verse_buf);

    return(0);
}

/* Place some verse at `line` in `verse` */
size_t seek_verse(char verse[], size_t line)
{
    FILE *bible;

    bible = fopen("bible.txt", "r");

    if (bible == NULL) 
    {
        puts("May God help us all.");
        return FALSE;
    }

    for (uint32_t i = 0; i < line - 1; ++i)
    {
        /* Throw away lines */
        get_line(verse, 0, bible);
    }
    get_line(verse, VERSE_SIZE, bible);

    fclose(bible);
    return(TRUE);
}

/* Cross platform getline() implentation that works with stack bufs */
size_t get_line(char extern_buf[], size_t len, FILE *stream)
{
    char buf[len];
    char character;

    /* If len <= 0, special behavior to skip line */
    if (len <= 0)
    {
        while ((character = getc(stream)) != EOF)
        {
            if (character == '\n')
                return(TRUE);
        }
        return(FALSE);
    }

    for (uint32_t i = 0; i < len; ++i)
    {
        if ((character = getc(stream)) != EOF)
        {
            buf[i] = character;
            if (character == '\n')
            {
                break;
            }
        } 
        else
        {
            break;
        }

    }
    buf[len] = '\0';

    strncpy(extern_buf, buf, len);
    return(TRUE);
}

/* Generate number from 1 to VERSE_COUNT (naively seeded) */
size_t generate_random(void)
{
    uint16_t random;
    uint16_t lower_bound = 3;

    srand(time(NULL));

    /* Grab random verse line (range safety cuz im paranoid) */
try_again:
    random = rand() % (VERSE_COUNT - lower_bound + 1) + lower_bound; 
    if (random <= 0 || random > VERSE_COUNT) goto try_again;

    return(random);
}