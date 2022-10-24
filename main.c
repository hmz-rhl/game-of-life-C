/**
 * @author Hamza RAHAL
 * 
*/


#include "libGOL.h"





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