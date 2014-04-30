#include "libtcod.hpp"
#include "Guard.h"
 
Guard::Guard(int x, int y, int ch, const TCODColor &col) :
    gX(x),gY(y),gCh(ch),gCol(col) {
}
 
void Guard::render() const {
    TCODConsole::root->setChar(gX,gY,gCh);
    TCODConsole::root->setCharForeground(gX,gY,gCol);
}

void Guard::moveUp(int mapWidth, int mapHeight, vector<char>element){
    /*if(gY > mapHeight - 1){
                    
        gY = 1;    
    } */
    if(gY>1) {
        if(element[(gY-1)*mapHeight + gX] != '#'){
            gY--;    
        }
    }
}


void Guard::moveDown(int mapWidth, int mapHeight, vector<char>element){
    /*if(gY==mapHeight-2) {
        moveUp(mapWidth, mapHeight, element);  
    } else {
        if(element[(gY+1) * mapHeight + gX] == '#'){
            moveUp(mapWidth, mapHeight, element);    
        }  
        gY++;
    }*/
    if(gY<mapHeight-2) {
        if(element[(gY+1)*mapHeight + gX] != '#'){
            gY++;    
        }
    }
}

void Guard::moveLeft(int mapWidth, int mapHeight, vector<char>element){
    /*if(element[gY * mapWidth + gX-1] == '#'){
        moveRight(mapWidth, mapHeight, element);    
    }  
    gX--;*/
    if(element[gY * mapWidth + gX-1] != '#'){
        gX--;    
    }
}

void Guard::moveRight(int mapWidth, int mapHeight, vector<char>element){
    /*if(element[gY * mapWidth + gX+1] == '#'){
        moveLeft(mapWidth, mapHeight, element);    
    }  
    gX++;*/
    if(element[gY * mapWidth + gX+1] != '#'){
        gX++;    
    }
}
