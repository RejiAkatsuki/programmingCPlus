#include "libtcod.hpp"
#include "Engine.h"
using namespace std;
#include "utility.h"
 
int main(int argc, char *argv[]) {

    if (argc != 2) {
		cout << "Invalid number of arguments.\n";
		cout << "Format of arguments example: task3 textfile.txt";
	} else {
        
        Engine engine(width, height, element, aX, aY, lX, lY, keyLeft, lifeLeft, point);
        engine.loadNewGame(argv[1]);
        //Engine engine;
        while ( !TCODConsole::isWindowClosed() ) {
        if(engine.gamePause){
            //if the game paused
            TCODConsole::root->clear();
            TCODConsole::root->print(1, 1, "Game Pause");
            TCODConsole::root->print(1, 4, "Esc - Exit, F1 - Continue, F2 - New Game");
            TCODConsole::flush();
            TCOD_key_t k;
            TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS,&k,NULL);
            switch(k.vk){
                case TCODK_ESCAPE :
                    //end game
                    exit(0);
                break;
                case TCODK_F1 :
                    //continute
                    engine.gamePause = false;
                break;
                case TCODK_F2 :
                    //new game 
                    {
                    engine.guards.clear();
                    engine.moneys.clear();
                    engine.keys.clear();
                    engine.logMessage.clear();
                    element.clear();
                    engine.width = 0; engine.height = 0;
                    engine.loadNewGame(argv[1]);
                    engine.player->aX = engine.oX;
                    engine.player->aY = engine.oY;
                    engine.gamePause = false;
                    }
                    
                break;  
                default:break;        
            }
        } else {
            engine.update();
            engine.render();
            TCODConsole::flush();
            }
        }

    }
    return 0;
}
