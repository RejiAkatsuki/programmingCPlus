#include "libtcod.hpp"
#include "Actor.h"
 
Actor::Actor(int x, int y, int ch, const TCODColor &col) :
    aX(x),aY(y),aCh(ch),aCol(col) {
}
 
void Actor::render() const {
    TCODConsole::root->setChar(aX,aY,aCh);
    TCODConsole::root->setCharForeground(aX,aY,aCol);
}
