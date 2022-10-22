/**
 * @author Hamza RAHAL
 * 
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <unistd.h>

#define MAX 512

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

int main()
{
    gameOfLife_t game;

    // speed of the simulation


    char input;
    initscr();			        /* Start curses mode 		        */
    
    noecho();                   /* don't see hited keys on term     */
    cbreak();
    curs_set(0);                /* make the cursor invisible        */

    // intialize the game at the init state
    game.state = INIT;

    // game loop
    while(1)
    {
        if(game.state == INIT)
        {
            initGOL(&game);             /* Initialize the game data
                                           get the initial patterns         */
            updateUI(&game);            /* Updating the user interface      */
            
        }

        else if(game.state == PAUSED)
        {
            // displaying this message
            strcpy(game.msgBox, "hit R to Resume\n\n\n");

            // updating the user interface
            updateUI(&game);

            input = getch();

            if(input == 'r' || input == 'R'){
                game.state = RUNNING;
            }
            // to temporize
            usleep(10000);
        }
 
        
        else if(game.state == RUNNING)
        {
            input = getch();

            // check if space is hitted
            if(input == SPACE){
                // changing the state of the game
                game.state = PAUSED;
            }
            // check if M is hitted to higher the animation
            else if (input == 'm' || input == 'M'){
                game.speed /= 1.5;
            }
            // check if L is hitted to lower the animation while higher than 1.0
            else if (input == 'l' || input == 'L'){
                game.speed = game.speed < 1.0 ? 1.0 : game.speed*1.5;
            }
            // check if x is hitted to exit
            else if(input == 'x' || input == 'x'){

                game.state = STOPPED;
            }
            
            usleep(game.speed*1000000);

            updateGOL(&game);
            updateUI(&game);
            
        }
        else if(game.state == BLINKING || game.state == STABLE)
        {
            // check if it already displayed, if not we display this message
            if(game.state == STABLE){
                if(strcmp(game.msgBox, "The game is stable if you want : to create a new pattern hit N, or hit X in order to exit\n_        _     _\n    | |      | |   | |\n ___| |_ __ _| |__ | | ___\n/ __| __/ _` | '_ \\| |/ _ \n\\__ \\ || (_| | |_) | |  __/\n|___/\\__\\__,_|_.__/|_|\\___|\n\n")){

                    strcpy(game.msgBox, "The game is stable if you want : to create a new pattern hit N, or hit X in order to exit\n_        _     _\n    | |      | |   | |\n ___| |_ __ _| |__ | | ___\n/ __| __/ _` | '_ \\| |/ _ \n\\__ \\ || (_| | |_) | |  __/\n|___/\\__\\__,_|_.__/|_|\\___|\n\n");
                }
            }
            // the same for blinking state
            else{
                if(strcmp(game.msgBox, "The game is blinking if you want : to create a new pattern hit N, or hit X in order to exit\n_     _ _       _    _\n| |   | (_)     | |  (_)\n| |__ | |_ _ __ | | ___ _ __   __ _\n| '_ \\| | | '_ \\| |/ / | '_ \\ / _` |\n| |_) | | | | | |   <| | | | | (_| |\n|_.__/|_|_|_| |_|_|\\_\\_|_| |_|\\__, |\n                               __/ |\n                               |___/\n")){

                    strcpy(game.msgBox, "The game is blinking if you want : to create a new pattern hit N, or hit X in order to exit\n_     _ _       _    _\n| |   | (_)     | |  (_)\n| |__ | |_ _ __ | | ___ _ __   __ _\n| '_ \\| | | '_ \\| |/ / | '_ \\ / _` |\n| |_) | | | | | |   <| | | | | (_| |\n|_.__/|_|_|_| |_|_|\\_\\_|_| |_|\\__, |\n                               __/ |\n                               |___/\n");     
                }
            }
            
            input = getch();

            if(input == 'N' || input == 'n'){
                game.state = INIT;
            }
            else if(input == 'X' || input == 'x'){
                game.state = STOPPED;
            }
            updateGOL(&game);
            updateUI(&game);
            usleep(100000);
            
        }
        else if (game.state == STOPPED)
        {
            refresh();			        /* Print it on to the real screen   */

                // close curse window
                endwin();	
                // break the loop
                break;
        }

    }
		        

    return EXIT_SUCCESS;
}

void initGOL(gameOfLife_t *game){

    // setting the cursor to the center of the board
    game->cursorX = SIZE/2;
    game->cursorY = SIZE/2;

    // initialize all variables
    game->generation = 0;

    game->nbAlive = 0;

    game->speed = 1.0;

    // no blocking mode for waiting an input

    nodelay(stdscr, FALSE);

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

        usleep(1000);

    }while(input != ENTER);

    game->state = RUNNING;

    // change to non-blocking mode(getch())
    nodelay(stdscr, TRUE);

}

void updateUI(gameOfLife_t *game){
    
    refresh();
    clear();

    printw(game->msgBox);

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
                        printw("|U|");
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
        
        game->generation++;
        sprintf(game->msgBox, "Alive %d: step %d\nHit M to speed up , or L to low the speed\nHit X to exit\n\n",game->nbAlive, game->generation);
        
        if(game->nbAlive + 1 == 0)
        {
            game->state = STOPPED;
        }
        
        // check if the cells are stable and displaying the correspondig message in that case
        if(isEqual(game->cell, game->previousCell))
        {
            game->state = STABLE;
            game->speed = 1.0;
            
        }
        // check if the cells are blinking and display a corresponding message in that case.
        else if(isEqual(game->cell, game->secondPreviousCell))
        {
            game->state = BLINKING;
            game->speed = 1.0;
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