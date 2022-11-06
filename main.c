/**
 * @author Hamza RAHAL
 * 
*/


#include "libGOL.h"






int main()
{
    // todo constant fps
    long long previous_timestamp = 0; 
    // instancing the game variable
    gameOfLife_t game;

    // speed of the simulation

    // variable for the user input
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
            sprintf(game.msgBox, "alive : %d \n hit R to Resume\n\n\n", game.nbAlive);

            

            input = getch();

            if(input == 'r' || input == 'R'){
                game.state = RUNNING;
                // updating the user interface
                updateUI(&game);
                

            }
             // to control the speed of the simulation
            if(current_timestamp() - previous_timestamp > game.speed )
            {
                updateUI(&game);
                
                previous_timestamp = current_timestamp();
            }
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
                game.speed = game.speed/1.5 < 100.0 ? 100.0 : game.speed/1.5;

            }
            // check if L is hitted to lower the animation while higher than 1.0
            else if (input == 'l' || input == 'L'){
                game.speed = game.speed*1.5 > 3500.0 ? 3500.0 : game.speed*1.5;

            }
            // check if x is hitted to exit
            else if(input == 'x' || input == 'x'){

                game.state = STOPPED;
            }
            
            // to control the speed of the simulation
            if(current_timestamp() - previous_timestamp > game.speed )
            {
                updateGOL(&game);
                updateUI(&game);
                previous_timestamp = current_timestamp();
            }
            //usleep(game.speed*1000000);

            
        }
        else if(game.state == BLINKING || game.state == STABLE)
        {
            
            input = getch();

            if(input == 'N' || input == 'n'){
                game.state = INIT;
            }
            else if(input == 'X' || input == 'x'){
                game.state = STOPPED;
            }
            // to control the speed of the simulation
            if(current_timestamp() - previous_timestamp > 3500)
            {
                updateGOL(&game);
                updateUI(&game);
                previous_timestamp = current_timestamp();
            }
            
            
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