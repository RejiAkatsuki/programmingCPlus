#ifndef COLLECTION_H
#define COLLECTION_H
#include "copy.h"

#include <QString>
#include <QList>
class Collection
{
private:
    QString title;
    QString type;
    QString version;
    int idcode;
    QString publisher;
    int year;
    QString duration;
    QString category;
    QList<Copy> copies;

public:
    Collection(QString title, QString type, QString version, int idcode, QString publisher, int year, QString duration, QString category):
        title(title),type(type),version(version),idcode(idcode),publisher(publisher),year(year),duration(duration),category(category)
        {}
    ~Collection();
    void addCopy(Copy copy);
    void removeCopy(Copy copy);
    QList<Copy> getCopy();

};

#endif // COLLECTION_H
