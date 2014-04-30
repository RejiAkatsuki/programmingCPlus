#include "mainwindow.h"
#include <QApplication>
#include <QtSql>
QSqlDatabase db;
int main(int argc, char *argv[])
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./library.sqlite");
    if( !db.open() )
    {
      qDebug() << db.lastError();
      qFatal( "Failed to connect." );
    }
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
