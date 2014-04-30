#include "libtcod.hpp"
#include "Money.h"
 
Money::Money(int x, int y, int ch, const TCODColor &col) :
    mX(x),mY(y),mCh(ch),mCol(col) {
}
 
void Money::render() const {
    TCODConsole::root->setChar(mX,mY,mCh);
    TCODConsole::root->setCharForeground(mX,mY,mCol);
}
