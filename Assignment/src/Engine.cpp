#include "libtcod.hpp"
#include "Engine.h"
using namespace std;
//#include "utility.h"

//constructor
Engine::Engine(int w, int h, vector<char> e, int aX, int aY, int lX, int lY, int k, int l, int p) 
{    
    width = w;
    height  = h;
    element = e;
    keyLeft = k;
    lifeLeft = l;
    point = p;
    level = 1;
    gamePause = false;
    
    TCODConsole::initRoot(80,50,"Lock & Chase",false);//draw consol(width, height, name of consol, resizeable)
}

//deconstructor
Engine::~Engine() {
    guards.clearAndDelete();
    delete player;
    delete map;
    delete lock;
    keys.clearAndDelete();
    moneys.clearAndDelete();
}

void Engine::addLogFile(string message){
    if(logMessage.size() == 5){
        logMessage.erase(logMessage.begin());    
    }
    logMessage.push_back(message);
    
}

void Engine::collision(){
    //check collision for Money
    for (Money **iterator=moneys.begin(); 
        iterator != moneys.end(); iterator++) {
        if(player->aX == (*iterator)->mX && player->aY == (*iterator)->mY){
            point += 100;
            addLogFile("Player get Money");
            iterator = moneys.remove(iterator);
        }
    }
    //check collision for Key
    for (Key **iterator=keys.begin(); 
        iterator != keys.end(); iterator++) {
        if(player->aX == (*iterator)->kX && player->aY == (*iterator)->kY){
            keyLeft--;
            point += 200;
            addLogFile("Player get Key");
            iterator = keys.remove(iterator);
        }
    }
    //check collision for Guard
    for (Guard **iterator=guards.begin(); 
        iterator != guards.end(); iterator++) {
        if(player->aX == (*iterator)->gX && player->aY == (*iterator)->gY){
            point -= 400;
            addLogFile("Player got Busted");
            lifeLeft--;
            player->aX = oX;
            player->aY = oY;
        }
    }
}

void Engine::saveGame(){
    char mapRecord[width*height];    

    for(int i = 0; i < width*height; i++){
        //make the space for element        
        mapRecord[i] = ' ';    
    }
    //rebuild #
    for(int i=0; i<height; i++) {
        for(int j=0; j<width; j++) {
            if(element[i*width + j] == '#'){
                mapRecord[i*width + j] = '#';            
            }
        }
    }
    //rebuild actor
    mapRecord[player->aY * width + player->aX] = '@';
    //rebuild lock
    mapRecord[lock->lY * width + lock->lX] = '\\';
    //rebuild guard
    for (Guard **iterator=guards.begin(); 
        iterator != guards.end(); iterator++) {
        mapRecord[(*iterator)->gY * width + (*iterator)->gX] = '^';
    }
    //rebuild key
    for (Key **iterator=keys.begin(); 
        iterator != keys.end(); iterator++) {
        mapRecord[(*iterator)->kY * width + (*iterator)->kX] = '+';
    }
    //rebuild money 
    for (Money **iterator=moneys.begin(); 
        iterator != moneys.end(); iterator++) {
        mapRecord[(*iterator)->mY * width + (*iterator)->mX] = '$';
    }  
    //write to file
    ofstream saveFile("autoSave.xml");
    if(saveFile.is_open()){
        for(int i=0; i<height; i++) {
            for(int j=0; j<width; j++) {
                saveFile<<mapRecord[i*width + j];
            }
        saveFile<<'\n';
        }
        saveFile<<keyLeft;
        saveFile<<'\n';
        saveFile<<lifeLeft;
        saveFile<<'\n';
        saveFile<<point;        
    }
}

//load the new game
void Engine::loadGame(){
    guards.clear();
    moneys.clear();
    keys.clear();
    logMessage.clear();
    width = 0; height = 0; //--> important: need to reset it so it can load the map 

    ifstream inputFile("autoSave.xml");
    int r;
    vector<int> saveRecord;
    vector<char> newElement;
    if(!inputFile.is_open()) 
    {
        cout << "Read File Fail!/ no more Map\n";
        TCODConsole::root->print(1, 1, "Read File Fail/ no more Map");
        exit(0);
    }
    else {
        string line;
        while(getline(inputFile, line)) {
            char *cstr = new char[line.length()+1];
            strcpy(cstr, line.c_str());
            if(cstr[0] == '#') {
                height++;
                width = line.length();
                for(int i=0; i<line.length();i++){
                    newElement.push_back(cstr[i]);
                    //draw player
                    if(cstr[i] == '@'){
                        player->aX = i;        //<-- new Just set new position for player depends on the saveMap.xml
                        player->aY = height -1;//<-- new                        
                    }
                    //draw lock
                    if(cstr[i] == '\\'){
                        lock->lX = i;          //<--- new Just set new position for lock depends on the saveMap.xml
                        lock->lY =  height -1; //<---new
                    }
                }            
            }else {
                stringstream(line) >> r;
                saveRecord.push_back(r);                
            }
        }
        keyLeft = saveRecord[0];
        lifeLeft = saveRecord[1];
        point = saveRecord[2];
        inputFile.close();
         
        //guard, key, money display    
        for(int i=0; i<height; i++) {
            for(int j=0; j<width; j++) {
                //push Guards
                if(newElement[i*width + j] == '^'){
                    guards.push(new Guard(j,i,'^', TCODColor::red));            
                } 
                //push Money
                if(newElement[i*width + j] == '$'){
                    moneys.push(new Money(j,i,'$', TCODColor::green));            
                }
                //push Key  
                if(newElement[i*width + j] == '+'){
                    keys.push(new Key(j,i,'+', TCODColor::yellow));            
                } 
            }
        }   
    }
}

bool Engine::gameOver(){
    //player lose the game
    if(lifeLeft == 0){
        return true;    
    } 
    return false;
}

bool Engine::winGame(){
    //player win the game
    if(player->aX == lock->lX && player->aY == lock->lY){
        if(keyLeft == 0){
            level ++;
            return true;
        }
    }
    return false;
}

//change level - import new map
void Engine::gameWinOperation(){
    string mapName = "mapLevel.xml";
    stringstream ss;
    ss << level;
    string str = ss.str();
    mapName.insert(8, str);
    char *mn = new char[mapName.length() + 1];
    strcpy(mn, mapName.c_str());
    //clear old map
    guards.clear();
    moneys.clear();
    keys.clear();
    logMessage.clear();
    element.clear();
    width = 0; height = 0;
    
    loadNewGame(mn);
}

void Engine::update() {

    TCOD_key_t key;
    TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS,&key,NULL);

    switch(key.vk) {
        case TCODK_UP :{ 
            if ( ! map->isWall(player->aX,player->aY-1)) {
                //guard move Up first
                for (Guard **iterator=guards.begin();iterator != guards.end(); iterator++) {
                    (*iterator)->moveDown(width, height, element);
                }

                player->aY--;
                addLogFile("Player move Up");
                collision();
                if(gameOver()){
                    addLogFile("Game Over");
                }
                if(winGame()){
                    addLogFile("congratulation, You Win");
                    gameWinOperation();
                }
            }
        }break;
        case TCODK_DOWN :{ 
            if ( ! map->isWall(player->aX,player->aY+1)) {
                //guard move Down first
                for (Guard **iterator=guards.begin();iterator != guards.end(); iterator++) {
                    (*iterator)->moveUp(width, height, element);
                }

                player->aY++;
                addLogFile("Player move Down");
                collision();
                if(gameOver()){
                    addLogFile("Game Over");
                }
                if(winGame()){
                    addLogFile("congratulation, You Win");
                    gameWinOperation();
                }
            }
        }break;
        case TCODK_LEFT :{
            if ( ! map->isWall(player->aX-1,player->aY)) {
                //guard move left first
                for (Guard **iterator=guards.begin();iterator != guards.end(); iterator++) {
                    (*iterator)->moveRight(width, height, element);
                }

                player->aX--;
                addLogFile("Player move Left");
                collision();
                if(gameOver()){
                    addLogFile("Game Over");
                }
                if(winGame()){
                    addLogFile("congratulation, You Win");
                    gameWinOperation();
                }
            }
        }break;
        case TCODK_RIGHT :{ 
            if ( ! map->isWall(player->aX+1,player->aY)) {
                //guard move Right first
                for (Guard **iterator=guards.begin();iterator != guards.end(); iterator++) {
                    (*iterator)->moveLeft(width, height, element);
                }

                player->aX++;
                addLogFile("Player move Right");
                collision();
                if(gameOver()){
                    addLogFile("Game Over");
                }
                if(winGame()){
                    addLogFile("congratulation, You Win");
                    gameWinOperation();
                }
        
            }
        }break;
        case TCODK_F1:
            saveGame();
            addLogFile("Game Save");
        break;
        case TCODK_F2:
            loadGame();
        break;
        case TCODK_ESCAPE:
            gamePause = true;    
        default:break;
    }
}

void Engine::render() {
    TCODConsole::root->clear();
    
    //Draw information
    TCODConsole::root->print(width + 2, 0, "Key Remain: %d", keyLeft);
    TCODConsole::root->print(width + 2, 1, "Life Remain: %d", lifeLeft);
    TCODConsole::root->print(width + 2, 2, "Point Collected: %d", point);
    TCODConsole::root->print(width + 2, 5, "Esc for Pause");
    TCODConsole::root->print(width + 2, 6, "F1 for quick save");
    TCODConsole::root->print(width + 2, 7, "F2 for quick Load");
    //draw Log
    for(int i = 0; i < logMessage.size(); i ++){
        TCODConsole::root->print(2, height + 2 + i, logMessage[i].c_str());    
    }
    
// draw the map
    map->render();

 //draw the Guards
    for (Guard **iterator=guards.begin(); 
        iterator != guards.end(); iterator++) {
        (*iterator)->render();
    }

//draw the Moneys
    for (Money **iterator=moneys.begin(); 
        iterator != moneys.end(); iterator++) {
        (*iterator)->render();
    }
//draw the Keys
    for (Key **iterator=keys.begin(); 
        iterator != keys.end(); iterator++) {
        (*iterator)->render();
    }
//render player
    player->render();
//render lock
    lock->render();
}

void Engine::loadNewGame(char * fileName){
    vector<int> records;
    int record;
    ifstream inputFile(fileName);
    if(!inputFile.is_open()) 
    {
        TCODConsole::root->print(1, 1, "Read File Fail/ no more Map");
        cout << "Read File Fail/ no more Map!\n";
        exit(2);
    }
    else {
        string line;
        while(getline(inputFile, line)) {
            char *cstr = new char[line.length()+1];
            strcpy(cstr, line.c_str());
            if(cstr[0] == '#') {
                height++;
                width = line.length();
                for(int i=0; i<line.length();i++){
                    element.push_back(cstr[i]);
                    //draw player
                    if(cstr[i] == '@'){
                        player = new Actor(i,height - 1,'@',TCODColor::white);
                        oX = i;
                        oY = height-1;    
                    }
                    //draw lock
                    if(cstr[i] == '\\'){
                        lock = new Lock(i, height - 1, '/', TCODColor::blue);                    
                    }
                }            
            }else {
                stringstream(line) >> record;
                records.push_back(record);                
            }
                        
        }
        inputFile.close();    
    }
    keyLeft = records[0];
    lifeLeft = records[1];
    point = records[2];

    for(int i=0; i<height; i++) {
        for(int j=0; j<width; j++) {
            //push Guards
            if(element[i*width + j] == '^'){
                guards.push(new Guard(j,i,'^', TCODColor::red));            
            } 
            //push Money
            if(element[i*width + j] == '$'){
                moneys.push(new Money(j,i,'$', TCODColor::green));            
            }
            //push Key  
            if(element[i*width + j] == '+'){
                keys.push(new Key(j,i,'+', TCODColor::yellow));            
            } 
        }
    }

    map = new Map(width, height, element);
    
    
    cout << keyLeft << " " << lifeLeft << " " << point << "\n";
    cout << "w = " << width << " height = " << height << "\n";
    for(int i=0; i<height; i++) {
        for(int j=0; j<width; j++) {
            cout << element[i*width + j];        
        }
        cout << "\n";
    }
    cout << "Load Map Complete" << endl;
}
