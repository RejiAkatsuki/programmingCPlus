#include "collection.h"
Collection::~Collection(){

}

void Collection::addCopy(Copy copy) {
    copies.append(copy);
}

void Collection::removeCopy(Copy copy) {

}

QList<Copy> Collection::getCopy() {
    return copies;
}
