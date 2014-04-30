#ifndef DATABASE_H
#define DATABASE_H
#include <QtSql>
class DB
{
private:
    QSqlDatabase db;
public:
    DB(){}
    ~DB();
    void addNewCollection();

};

#endif // DATABASE_H
