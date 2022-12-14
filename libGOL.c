#include "libGOL.h"


void initGOL(gameOfLife_t *game){

    // setting the cursor to the center of the board
    game->cursorX = SIZE/2;
    game->cursorY = SIZE/2;

    // initialize all variables
    game->generation = 0;

    game->nbAlive = 0;

    game->speed = 500.0;

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
        
        // check the keys and move the cursor

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

    // display the new message box
    printw((const char *)(game->msgBox));

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
                        printw("|0|");
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
            // if we're on simulation we show only the cells and not cursor
            else{

                if(game->cell[row][col] == ALIVE)
                {
                    if(col == 0){
                        printw("|# ");

                    }
                    else if(col == SIZE - 1){

                        printw(" #|");
                    }
                    else{
                        printw(" # ");
                    }
                    
                }
                else
                {
                    
                    if(col == 0){
                        if(row == SIZE - 1){

                            printw("|__");
                        }
                        else{
                            printw("|  ");
                        }

                    }
                    else if(col == SIZE - 1){

                        if(row == SIZE - 1){

                            printw("__|");
                        }
                        else{
                            printw("  |");
                        }
                    }
                    else if(row == SIZE - 1){
                       printw("___");
                    }
                    else{
                        printw("   ");
                    }
                }
            }
            
        }   
        printw("\n");
    }
    
}

void updateGOL(gameOfLife_t *game){

    // storing the game variable before change to compare with the new one for detecting blinking or stable
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
            // corner up left
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
            // corner down left
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
            // corner up right
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
            // corner down right
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
            // first column
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
            // last column
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
            // first row
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
            // last row
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
        sprintf(game->msgBox, "Alive %d: step %d\t\t %0.2f Generation/sec\nHit M to speed up , or L to low the speed, or SPACE to pause the simulation\nHit X to exit\n\n",game->nbAlive, game->generation, 1000.0/game->speed);
        
        if(game->nbAlive + 1 == 0)
        {
            game->state = STOPPED;
        }
        
        // check if the cells are stable and displaying the correspondig message in that case
        if(isEqual(game->cell, game->previousCell))
        {
            game->state = STABLE;
            game->speed = 1.0;
            strcpy(game->msgBox, "The game is stable if you want : to create a new pattern hit N, or hit X in order to exit\n     _        _     _\n    | |      | |   | |\n ___| |_ __ _| |__ | | ___\n/ __| __/ _` | '_ \\| |/ _ \\\n\\__ \\ || (_| | |_) | |  __/\n|___/\\__\\__,_|_.__/|_|\\___|\n\n");
            
        }
        // check if the cells are blinking and display a corresponding message in that case.
        else if(isEqual(game->cell, game->secondPreviousCell))
        {
            game->state = BLINKING;
            game->speed = 1.0;
            strcpy(game->msgBox, "The game is blinking if you want : to create a new pattern hit N, or hit X in order to exit\n _     _ _       _    _\n| |   | (_)     | |  (_)\n| |__ | |_ _ __ | | ___ _ __   __ _\n| '_ \\| | | '_ \\| |/ / | '_ \\ / _` |\n| |_) | | | | | |   <| | | | | (_| |\n|_.__/|_|_|_| |_|_|\\_\\_|_| |_|\\__, |\n                               __/ |\n                               |___/\n");     

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


long long current_timestamp() {
    struct timeval te; 
    gettimeofday(&te, NULL); // get current time
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // calculate milliseconds
    // printf("milliseconds: %lld\n", milliseconds);
    return milliseconds;
}