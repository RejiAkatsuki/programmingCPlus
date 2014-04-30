#include "libtcod.hpp"
#include "Lock.h"
 
Lock::Lock(int x, int y, int ch, const TCODColor &col) :
    lX(x),lY(y),lCh(ch),lCol(col) {
}
 
void Lock::render() const {
    TCODConsole::root->setChar(lX,lY,lCh);
    TCODConsole::root->setCharForeground(lX,lY,lCol);
}
