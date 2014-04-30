#include "libtcod.hpp"
#include "map.h"
//#include "utility.h"

Map::Map(int w, int h, vector<char> e) {
    width = w; height = h;
    element = e;

    tiles=new Tile[width*height];
    for(int j =0; j < height; j ++){
        for(int i=0; i < width; i++){
        //calculate position
            if(element[j*width + i] == '#'){
                setWall(i,j);            
            }        
        }    
    }
}

Map::~Map() {
    delete [] tiles;
}

bool Map::isWall(int x, int y) const {
    return !tiles[x+y*width].canWalk;
}
 
void Map::setWall(int x, int y) {
    tiles[x+y*width].canWalk=false;
}

void Map::render() const {

for (int x=0; x < width; x++) {
    for (int y=0; y < height; y++) {
        TCODConsole::root->putChar(x,y,isWall(x,y) ? '#':' ');
    }
}
}
