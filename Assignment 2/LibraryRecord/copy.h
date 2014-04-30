#ifndef COPY_H
#define COPY_H

#include <QString>

class Copy
{
private:
    QString barcode;
    QString status;
    int borrowed;
    int returned;
    QString borrowDate;
    int numberOfDayBorrowed;

    int collection_id;

public:
    Copy();
    ~Copy();



};

#endif // COPY_H
