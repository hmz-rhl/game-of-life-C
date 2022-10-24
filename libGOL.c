#include "libGOL.h"


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