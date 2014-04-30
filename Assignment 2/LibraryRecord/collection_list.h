#ifndef COLLECTION_LIST_H
#define COLLECTION_LIST_H
#include "collection.h"
#include <QList>
class CollectionList
{
private:
    QList<Collection> collections;
public:
    CollectionList(){collections.clear();}
    ~CollectionList();
    void addCollection(Collection collection);
    void removeCollection(Collection collection);
    Collection findCollectionById(int id);


};

#endif // COLLECTION_LIST_H
