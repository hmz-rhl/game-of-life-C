#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

// macro for size of the game 
#define SIZE 20

// macro for state of the cells
#define ALIVE 1
#define DEAD 0

// structure of a cell
typedef struct cell
{
    int neighboursAlive;
    int state;

} cell_t;

//structure of the game
typedef struct gameOfLife
{
    cell_t cell[SIZE][SIZE];
    int state;

} gameOfLife_t;

/**
 * @brief function that init our variable by interact graphicalliy with user to set up yhe intial pattern. must click to change state of cell
 * and load the pattern when it hits enter. Must be called once at the start end of the game.
 * 
 * @param game a pointer to the game variable wich will store all game data
 * 
**/
void initGOL(gameOfLife_t *game);

/**
 * @brief function that update the state of each cell. Must be called after each loop.
 * 
 * @param game a pointer to the game variable wich will store all game data.
 * 
**/
void updateGOL(gameOfLife_t *game);

/**
 * @brief function that update the ncurses windows to follow of each new state of the game. Must be called after each loop.
 * 
 * @param game a pointer to the game variable wich will store all game data
 * 
**/
void updateUI(gameOfLife_t *game);


int main()
{
    gameOfLife_t game;
    
    initscr();			        /* Start curses mode 		        */
	printw("Hello World !!!");	/* Print Hello World		        */
	refresh();			        /* Print it on to the real screen   */
	getch();			        /* Wait for user input              */
	endwin();			        /* End curses mode		            */

    return EXIT_SUCCESS;
}