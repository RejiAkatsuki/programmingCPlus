#ifndef BORROWER_LIST_H
#define BORROWER_LIST_H
#include "borrower.h"
#include <QList>

class BorrowerList
{
private:
    QList<Borrower> borrowers;
public:
    BorrowerList();
    ~BorrowerList();
    void addBorrower(Borrower borrower);
    void removeBorrower(Borrower borrower);
    Borrower findBorrowerById(int id);
};
#endif // BORROWER_LIST_H
