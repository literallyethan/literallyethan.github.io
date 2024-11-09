#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

#define VERSE_COUNT 31102
#define VERSE_SIZE 500
#define TRUE 1
#define FALSE 0

int seek_verse(char verse[]);

/* Read a verse from bible.txt randomly and print it. */
int main(void)
{
    char verse_buf[VERSE_SIZE];

    /* Choose some verse at random */
    if(!seek_verse(verse_buf))
    {
        puts("It's the Bible, you get credit for trying.");
        exit(-1);
    }

    return(0);
}

int seek_verse(char verse[])
{
    FILE* bible;
    uint16_t random;
    uint16_t lower_bound = 1;

    srand(time(NULL));


    random = rand() % (VERSE_COUNT - lower_bound + 1) + lower_bound; 
    bible = fopen("bible.txt", "r");

    if (bible == NULL) 
    {
        puts("May God help us all.");
        return FALSE;
    }

    printf("max rand number: %d\n", random); 

    fclose(bible);
    return(TRUE);
}