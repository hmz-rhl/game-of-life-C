/**
 * @author
 * 
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <unistd.h>

#define MAX 256

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

#define INIT 2
#define RUNNING 1
#define STOPPED 0
#define EXIT -1
#define PAUSED -2

// structure of a cell
typedef struct cell
{
    int state;

} cell_t;

//structure of the game
typedef struct gameOfLife
{
    int cell[SIZE][SIZE];
    int previousCell[SIZE][SIZE];
    int secondPreviousCell[SIZE][SIZE];
    int state;
    int cursorX;
    int cursorY;
    char msgBox[MAX];
    int generation;
    int nbAlive;

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

int main()
{
    gameOfLife_t game;

    // speed of the simulation
    float speed = 1.0;

    char input;
    initscr();			        /* Start curses mode 		        */
    
    noecho();                   /* don't see hited keys on term*    */
    cbreak();

    initGOL(&game);
    updateUI(&game);

    while(game.state == RUNNING)
    {
        updateGOL(&game);
        updateUI(&game);
        usleep(speed*1000000);
        input = getch();

        // a revoir
        if(input == SPACE){

            // waiting for releasing space key
            do{
                input = getch();
                usleep(10000);

            }while(input == SPACE);

            game.state = PAUSED;
            strcpy(game.msgBox, "hit SPACE to Resume\n\n\n");
            updateUI(&game);

            // waiting for hiting space key again
            do
            {
                input = getch();
                usleep(10000);

            }while (input != SPACE);
            
            game.state = RUNNING;
            updateUI(&game);
        }
        else if (input == 'm' || input == 'M'){
            speed /= 1.5;
        }
        else if (input == 'l' || input == 'L'){
            speed *= 1.5;
        }
        else if(input == 'x' || input == 'x'){

            refresh();			        /* Print it on to the real screen   */
            // close curse window
            endwin();	
            break;
        }
    }
		        

    return EXIT_SUCCESS;
}

void initGOL(gameOfLife_t *game){

    game->cursorX = SIZE/2;
    game->cursorY = SIZE/2;

    game->state = INIT;
    game->generation = 0;

    game->nbAlive = 0;

    // set the msgBox
    sprintf(game->msgBox,"Alive : %d\nHit arrows to move the cursor -/+, hit space to change the state of the cell\nThen when you're done, hit enter to start simulation\nHit X to exit\n", game->nbAlive);

    for (int i = 0; i < SIZE -1; i++)
    {
        for (int j = 0; j < SIZE -1; j++)
        {
            game->cell[i][j] = DEAD;
            game->previousCell[i][j] = DEAD;
            game->secondPreviousCell[i][j] = DEAD;
            
        }
    }
    
    // generate and show the UI
    updateUI(game);
    
    // waiting for key to be pressed
    char input = getch();

    do{
        
        if(input == UP)
        {
            if(game->cursorY > 0)
            {
                game->cursorY --;
            }
        }
        else if(input == DOWN)
        {
            if(game->cursorY < SIZE - 1)
            {
                game->cursorY ++;
            }
        }
        else if(input == LEFT)
        {
            if(game->cursorX > 0)
            {
                game->cursorX --;
            }
        }
        else if(input == RIGHT)
        {
            if(game->cursorX < SIZE - 1)
            {
                game->cursorX ++;
            }
        }
        else if(input == SPACE)
        {

            game->cell[game->cursorY][game->cursorX] = 1 - game->cell[game->cursorY][game->cursorX];
            
            if(game->cell[game->cursorY][game->cursorX] == ALIVE)
            {
                game->nbAlive+=1;
            }
            else
            {
                game->nbAlive-=1;

            }
            // refresh the new msgBox (nbAlive)
            sprintf(game->msgBox,"Alive : %d\nHit arrows to move the cursor -/+, hit space to change the state of the cell\nThen when you're done, hit enter to start simulation\nHit X to exit\n", game->nbAlive);

        }
        
        else if (input == 'x' || input == 'X')
        {
            game->state = EXIT;
            endwin();
            exit(EXIT_SUCCESS);
        }

        updateUI(game);
        input = getch();

    }while(input != ENTER);

    game->state = RUNNING;
    // change to non-blocking mode(getch())
    nodelay(stdscr, TRUE);

}

void updateUI(gameOfLife_t *game){
    
    refresh();
    clear();

    printw(game->msgBox);

    if(game->state == PAUSED){

        printw("state : PAUSED !\n");
    }
    else if(game->state == RUNNING){

        if(isEqual(game->cell, game->previousCell))
        {
            printw("     _        _     _\n    | |      | |   | |\n ___| |_ __ _| |__ | | ___\n/ __| __/ _` | '_ \\| |/ _ \n\\__ \\ || (_| | |_) | |  __/\n|___/\\__\\__,_|_.__/|_|\\___|\n\n");
        }
        else if(isEqual(game->cell, game->secondPreviousCell))
        {

            printw(" _     _ _       _    _\n| |   | (_)     | |  (_)\n| |__ | |_ _ __ | | ___ _ __   __ _\n| '_ \\| | | '_ \\| |/ / | '_ \\ / _` |\n| |_) | | | | | |   <| | | | | (_| |\n|_.__/|_|_|_| |_|_|\\_\\_|_| |_|\\__, |\n                               __/ |\n                               |___/\n");     
        }


    }

    for(int row = 0; row<SIZE; row++)
    {
        for(int col = 0; col<SIZE; col++)
        {
            // if we're on initialisation
            if(game->state == INIT)
            {
                if(row == game->cursorY && col == game->cursorX){

                    if(game->cell[row][col] == ALIVE)
                    {
                        printw("|+|");
                    }
                    else
                    {
                        printw("|-|");
                    }
                }
                else{
                        if(game->cell[row][col] == ALIVE)
                        {
                            printw("|#|");
                        }
                        else
                        {
                            printw("|_|");
                        }
                }
            }
            // if we're on simulation
            else if(game->state == RUNNING || game->state == PAUSED){

                if(game->cell[row][col] == ALIVE)
                {
                    printw("|#|");
                }
                else
                {
                    printw("|_|");
                }
            }
            
        }   
        printw("\n");
    }
    
}

void updateGOL(gameOfLife_t *game){

    gameOfLife_t current = *game;


   
    for(int row = 0; row<SIZE; row++)
    {
        for(int col = 0; col<SIZE; col++)
        {
            // we treat cells which are away from border
            if(col > 1 && row >1 && col < SIZE -1 && row < SIZE -1)
            {
                //overpopulation
                if(current.cell[row+1][col] +current.cell[row-1][col] +current.cell[row][col+1] +current.cell[row][col-1] +current.cell[row-1][col+1] +current.cell[row+1][col-1] +current.cell[row+1][col+1] +current.cell[row-1][col-1] > 3 && current.cell[row][col] == ALIVE)
                {
                    game->cell[row][col] = DEAD;
                    game->nbAlive--;
                }
                //underpopulation
                else if(current.cell[row+1][col] +current.cell[row-1][col] +current.cell[row][col+1] +current.cell[row][col-1] +current.cell[row-1][col+1] +current.cell[row+1][col-1] +current.cell[row+1][col+1] +current.cell[row-1][col-1] < 2 && current.cell[row][col] == ALIVE)
                {
                    game->cell[row][col] = DEAD;
                    game->nbAlive--;
                }
                // birth
                if(current.cell[row+1][col] +current.cell[row-1][col] +current.cell[row][col+1] +current.cell[row][col-1] +current.cell[row-1][col+1] +current.cell[row+1][col-1] +current.cell[row+1][col+1] +current.cell[row-1][col-1] == 3 && current.cell[row][col] == DEAD)
                {
                    game->cell[row][col] = ALIVE;
                    game->nbAlive++;
                }

            }
            else if(col == 0 && row == 0)
            {
                //overpopulation
                if(current.cell[row+1][col] +current.cell[row][col+1] +current.cell[row+1][col+1] > 3 && current.cell[row][col] == ALIVE)
                {
                    game->cell[row][col] = DEAD;
                    game->nbAlive--;
                }
                //underpopulation
                else if(current.cell[row+1][col] +current.cell[row][col+1] +current.cell[row+1][col+1] < 2 && current.cell[row][col] == ALIVE)
                {
                    game->cell[row][col] = DEAD;
                    game->nbAlive--;
                }
                // birth
                if( current.cell[row+1][col] +current.cell[row][col+1] +current.cell[row+1][col+1] == 3 && current.cell[row][col] == DEAD)
                {
                    game->cell[row][col] = ALIVE;
                    game->nbAlive++;
                }
            
            }
            else if(col == 0 && row == SIZE-1)
            {
                //overpopulation
                if(current.cell[row-1][col] +current.cell[row][col+1] +current.cell[row-1][col+1] > 3 && current.cell[row][col] == ALIVE)
                {
                    game->cell[row][col] = DEAD;
                    game->nbAlive--;
                }
                //underpopulation
                else if(current.cell[row-1][col] +current.cell[row][col+1] +current.cell[row-1][col+1] < 2 && current.cell[row][col] == ALIVE)
                {
                    game->cell[row][col] = DEAD;
                    game->nbAlive--;
                }
                // birth
                if( current.cell[row-1][col] +current.cell[row][col+1] +current.cell[row-1][col+1] == 3 && current.cell[row][col] == DEAD)
                {
                    game->cell[row][col] = ALIVE;
                    game->nbAlive++;
                }
            }
            else if(col == SIZE - 1 && row == 0)
            {
                //overpopulation
                if(current.cell[row+1][col] +current.cell[row][col-1] +current.cell[row+1][col-1] > 3 && current.cell[row][col] == ALIVE)
                {
                    game->cell[row][col] = DEAD;
                    game->nbAlive--;
                }
                //underpopulation
                else if(current.cell[row+1][col] +current.cell[row][col-1] +current.cell[row+1][col-1] < 2 && current.cell[row][col] == ALIVE)
                {
                    game->cell[row][col] = DEAD;
                    game->nbAlive--;
                }
                // birth
                if(current.cell[row+1][col] +current.cell[row][col-1] +current.cell[row+1][col-1]  == 3 && current.cell[row][col] == DEAD)
                {
                    game->cell[row][col] = ALIVE;
                    game->nbAlive++;
                }
            }
            else if(col == SIZE - 1 && row == SIZE -1)
            {
                //overpopulation
                if(current.cell[row-1][col] +current.cell[row][col-1] +current.cell[row-1][col-1] > 3 && current.cell[row][col] == ALIVE)
                {
                    game->cell[row][col] = DEAD;
                    game->nbAlive--;
                }
                //underpopulation
                else if(current.cell[row-1][col] +current.cell[row][col-1] +current.cell[row-1][col-1] < 2 && current.cell[row][col] == ALIVE)
                {
                    game->cell[row][col] = DEAD;
                    game->nbAlive--;
                }
                // birth
                if( current.cell[row-1][col] +current.cell[row][col-1] +current.cell[row-1][col-1] == 3 && current.cell[row][col] == DEAD)
                {
                    game->cell[row][col] = ALIVE;
                    game->nbAlive++;
                }

            }
            else if(col == 0)
            {
                //overpopulation
                if(current.cell[row-1][col] +current.cell[row+1][col] +current.cell[row-1][col+1] +current.cell[row+1][col+1] +current.cell[row][col+1] > 3 && current.cell[row][col] == ALIVE)
                {
                    game->cell[row][col] = DEAD;
                    game->nbAlive--;
                }
                //underpopulation
                else if(current.cell[row-1][col] +current.cell[row+1][col] +current.cell[row-1][col+1] +current.cell[row+1][col+1] +current.cell[row][col+1] < 2 && current.cell[row][col] == ALIVE)
                {
                    game->cell[row][col] = DEAD;
                    game->nbAlive--;
                }
                // birth
                if(current.cell[row-1][col] +current.cell[row+1][col] +current.cell[row-1][col+1] +current.cell[row+1][col+1] +current.cell[row][col+1] == 3 && current.cell[row][col] == DEAD)
                {
                    game->cell[row][col] = ALIVE;
                    game->nbAlive++;
                }
                
            }
            else if(col == SIZE -1)
            {
                //overpopulation
                if(current.cell[row-1][col] +current.cell[row+1][col] +current.cell[row-1][col-1] +current.cell[row+1][col-1] +current.cell[row][col-1] > 3 && current.cell[row][col] == ALIVE)
                {
                    game->cell[row][col] = DEAD;
                    game->nbAlive--;
                }
                //underpopulation
                else if(current.cell[row-1][col] +current.cell[row+1][col] +current.cell[row-1][col-1] +current.cell[row+1][col-1] +current.cell[row][col-1] < 2 && current.cell[row][col] == ALIVE)
                {
                    game->cell[row][col] = DEAD;
                    game->nbAlive--;
                }
                // birth
                if(current.cell[row-1][col] +current.cell[row+1][col] +current.cell[row-1][col-1] +current.cell[row+1][col-1] +current.cell[row][col-1] == 3 && current.cell[row][col] == DEAD)
                {
                    game->cell[row][col] = ALIVE;
                    game->nbAlive++;
                }
            }
            else if(row == 0)
            {
                //overpopulation
                if(current.cell[row+1][col] +current.cell[row+1][col+1] +current.cell[row+1][col-1] +current.cell[row][col+1] +current.cell[row][col-1] > 3 && current.cell[row][col] == ALIVE)
                {
                    game->cell[row][col] = DEAD;
                    game->nbAlive--;
                }
                //underpopulation
                else if(current.cell[row+1][col] +current.cell[row+1][col+1] +current.cell[row+1][col-1] +current.cell[row][col+1] +current.cell[row][col-1] < 2 && current.cell[row][col] == ALIVE)
                {
                    game->cell[row][col] = DEAD;
                    game->nbAlive--;
                }
                // birth
                if(current.cell[row+1][col] +current.cell[row+1][col+1] +current.cell[row+1][col-1] +current.cell[row][col+1] +current.cell[row][col-1] == 3 && current.cell[row][col] == DEAD)
                {
                    game->cell[row][col] = ALIVE;
                    game->nbAlive++;
                }
            }
            else if(row == SIZE -1)
            {
                //overpopulation
                if(current.cell[row-1][col] +current.cell[row-1][col+1] +current.cell[row-1][col-1] +current.cell[row][col+1] +current.cell[row][col-1] > 3 && current.cell[row][col] == ALIVE)
                {
                    game->cell[row][col] = DEAD;
                    game->nbAlive--;
                }
                //underpopulation
                else if(current.cell[row-1][col] +current.cell[row-1][col+1] +current.cell[row-1][col-1] +current.cell[row][col+1] +current.cell[row][col-1] < 2 && current.cell[row][col] == ALIVE)
                {
                    game->cell[row][col] = DEAD;
                    game->nbAlive--;
                }
                //birth
                if(current.cell[row-1][col] +current.cell[row-1][col+1] +current.cell[row-1][col-1] +current.cell[row][col+1] +current.cell[row][col-1] == 3 && current.cell[row][col] == DEAD)
                {
                    game->cell[row][col] = ALIVE;
                    game->nbAlive++;
                }
            }
            
           
            // saving the old states
            game->previousCell[row][col] = current.cell[row][col];
            game->secondPreviousCell[row][col] = current.previousCell[row][col];
        }


        
    }


    
    if(game->state == RUNNING)
    {
        //TODO
        game->generation++;
        sprintf(game->msgBox, "Alive %d: step %d\nHit M to speed up , or L to low the speed\nHit X to exit\n\n",game->nbAlive, game->generation);
        
        if(game->nbAlive + 1 == 0)
        {
            game->state = STOPPED;
        }
        
    }
}


int isEqual(int a[SIZE][SIZE], int b[SIZE][SIZE]){

    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if(a[i][j] != b[i][j])
            {
                return 0;
            }
        }
        
    }

    return 1;
    
}