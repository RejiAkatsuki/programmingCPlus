#include <vector>
#include "Actor.h"
#include "map.h"
#include "Guard.h"
#include "Money.h"
#include "Lock.h"
#include "Key.h"
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

class Engine {
public :
    vector<char> element;
    int width, height;
    int keyLeft, lifeLeft, point;
    int oX, oY;
    TCODList<Guard *> guards;
    TCODList<Key *> keys;
    TCODList<Money *> moneys;
    Actor *player;
    Lock *lock;
    Map *map;
    vector<string>logMessage;
 
    Engine(int, int, vector<char>, int, int, int, int, int, int, int);
    Engine();
    ~Engine();
    int level;
    void update();
    void render();
    void collision();
    void addLogFile(string);
    void saveGame();
    void loadGame();
    bool gameOver();
    bool winGame();
    bool gamePause;
    void loadNewGame(char * fileName);
    void gameWinOperation();
    void guardMoveLeftRight();
};
extern Engine engine; 

