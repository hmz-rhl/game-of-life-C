// all definition of our game

// necessar libraries
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/time.h>


// defining the macros
#define MAX 512

// mcro for the user inputs
#define UP 65
#define DOWN 66
#define RIGHT 67
#define LEFT 68
#define SPACE 32
#define ENTER '\n'



// macro for size of the game 
#define SIZE 40

// macro for state of the cells
#define ALIVE 1
#define DEAD 0


// macro for the game state
#define INIT 2
#define RUNNING 1
#define STOPPED 0
#define EXIT -1
#define PAUSED -2
#define STABLE -3
#define BLINKING -4

// structure of a cell
typedef struct cell
{
    int state;

} cell_t;

//structure of the game
typedef struct gameOfLife
{
    int cell[SIZE][SIZE];                   /**/
    int previousCell[SIZE][SIZE];           /**/
    int secondPreviousCell[SIZE][SIZE];     /**/
    int state;                              /* state of the game : RUNNING PAUSED STOPPED EXIT INIT*/
    float speed;
    int cursorX;                            /* cursor positions :                                           */
    int cursorY;                            /* to display it on the screen and know the user selections)    */

    char msgBox[MAX];                       /* stores the content to display on the message box             */
    int generation;                         /* stores the generation of the game of life array              */
    int nbAlive;                            /* stores the alive cell to display it on the message box       */

} gameOfLife_t;

/**
 * @brief function that init our variable by interact graphicalliy with user to set up yhe intial pattern. must hit keyboard arrows and space to change the state of cells
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

/**
 * @brief check if two double array are equal
 * 
 * @param a a pointer to fisrt array
 * 
 * @param b a pointer to the second array
 * 
 * @param size size of arrays
 * 
 * @return 1 if equal, 0 if not
 * 
**/
int isEqual(int a[SIZE][SIZE], int b[SIZE][SIZE]);

/**
 * @brief function that return the current timestamp in ms
 * 
 * @return the time in ms
 * 
**/
long long current_timestamp(void);