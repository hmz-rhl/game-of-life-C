#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#define SIZE 20

typedef struct cell
{
    int neighboursAlive;
    int state;

} cell_t;

typedef struct gameOfLife
{
    cell_t cell[SIZE][SIZE];
    int state;

} gameOfLife_t;

void update(gameOfLife_t *);

int main()
{
    gameOfLife_t game;
    
    initscr();			/* Start curses mode 		  */
	printw("Hello World !!!");	/* Print Hello World		  */
	refresh();			/* Print it on to the real screen */
	getch();			/* Wait for user input */
	endwin();			/* End curses mode		  */

    return EXIT_SUCCESS;
}