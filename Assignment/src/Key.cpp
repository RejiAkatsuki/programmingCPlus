#include "libtcod.hpp"
#include "Key.h"
 
Key::Key(int x, int y, int ch, const TCODColor &col) :
    kX(x),kY(y),kCh(ch),kCol(col) {
}
 
void Key::render() const {
    TCODConsole::root->setChar(kX,kY,kCh);
    TCODConsole::root->setCharForeground(kX,kY,kCol);
}
