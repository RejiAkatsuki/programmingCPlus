#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "borrowergui.h"
#include "library.h"
#include <QGraphicsView>
#include <QRect>
#include <QDesktopWidget>
#include <QGraphicsEffect>
#include <QPropertyAnimation>
#include <QtSql>
#include <QDir>

static bool checkHide;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //set middle position for display screen
    QRect position = frameGeometry();
    position.moveCenter(QDesktopWidget().availableGeometry().center());
    move(position.topLeft());
    checkHide = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnExit_clicked()
{
    exit(0);
}


void MainWindow::on_btnLibrary_clicked()
{
    Library *lib = new Library();
    lib->show();
    this->hide();
}

void MainWindow::on_btnBorrower_clicked()
{
    BorrowerGui *bor = new BorrowerGui();
    bor->show();
    this->hide();
}

void MainWindow::on_btnReport_clicked()
{
    QSqlQuery qry;
    QFile outputFile("output.html");
    outputFile.open(QIODevice::WriteOnly|QIODevice::Text);
    QTextStream out(&outputFile);
    out<<"<!DOCTYPE html>\n";
    out<<"<html>\n";
    out<<"<head><title>Library Report</title></head>\n";
    out<<"<body>\n";
    out << "<h1 style='color:red'>Library Report</h1>\n";
    out << "<p>To print, press ctrl-P or select menu File > Print</p>\n";
    out << "<p>To save to file, press ctrl-s or select menu File > Save</p>\n<br>";

    //Out collections list
    out << "<h1>List of Collections</h1>\n";
    out << "<table border='1'>\n<tr><th>ID</th><th>Title</th><th>ID Code</th><th>Type</th><th>Data Type</th><th>Version</th><th>Publisher</th><th>Year</th><th>Duration</th><th>Category</th></tr>";
    qry.prepare( "SELECT * FROM collections");
    if( !qry.exec() )
        qDebug() << qry.lastError();
    else
    {
        while (qry.next()) {
            out<<"<tr><td>"<<qry.value("id").toString()<<"</td><td>"<<qry.value("title").toString()<<"</td><td>"<<qry.value("idcode").toString()<<"</td><td>"<<qry.value("type").toString()<<"</td><td>"<<qry.value("datatype").toString()<<"</td><td>"<<qry.value("version").toString()<<"</td><td>"<<qry.value("publisher").toString()<<"</td><td>"<<qry.value("year").toString()<<"</td><td>"<<qry.value("duration").toString()<<"</td><td>"<<qry.value("category").toString()<<"</td></tr>\n";
        }
    }
    out << "</table>\n";

    //Out Copy List
    out << "<h1>List of Copy</h1>\n";
    out << "<table border='1'>\n<tr><th>ID</th><th>Barcode</th><th>Status</th><th>Times Borrowed</th><th>Borrow Date</th><th>Days borrowed</th><th>Collection</th><th>Borrower</th></tr>";
    qry.prepare( "SELECT co.id,barcode,status,times_borrow,borrow_date,days_borrowed,title,name FROM copy co LEFT JOIN collections c ON c.id = collection_id LEFt JOIN borrowers b ON b.id=borrowed_by");
    if( !qry.exec() )
        qDebug() << qry.lastError();
    else
    {
        while (qry.next()) {
            out<<"<tr><td>"<<qry.value("id").toString()<<"</td><td>"<<qry.value("barcode").toString()<<"</td><td>"<<qry.value("status").toString()<<"</td><td>"<<qry.value("times_borrow").toString()<<"</td><td>"<<qry.value("borrow_date").toString()<<"</td><td>"<<qry.value("days_borrowed").toString()<<"</td><td>"<<qry.value("title").toString()<<"</td><td>"<<qry.value("name").toString()<<"</td></tr>\n";
        }
    }
    out << "</table>\n<br>";


    //Out borrower List
    out << "<h1>List of Borrowers</h1>\n";
    out << "<table border='1'>\n<tr><th>ID</th><th>Name</th><th>Phone</th><th>Type</th><th>Department</th><th>Item borrowed</th><th>Item late</th></tr>";
    qry.prepare( "SELECT * FROM borrowers");
    if( !qry.exec() )
        qDebug() << qry.lastError();
    else
    {
        while (qry.next()) {
            out<<"<tr><td>"<<qry.value("id").toString()<<"</td><td>"<<qry.value("name").toString()<<"</td><td>"<<qry.value("phone").toString()<<"</td><td>"<<qry.value("type").toString()<<"</td><td>"<<qry.value("department").toString()<<"</td><td>"<<qry.value("items_borrowed").toString()<<"</td><td>"<<qry.value("items_late").toString()<<"</td></tr>\n";
        }
    }
    out << "</table>\n";

    out<<"</body>\n</html>";
    outputFile.close();
    QString filePath = "file://";
    filePath += QDir::currentPath();
    filePath +="/output.html";
    qDebug() << filePath;
    QDesktopServices::openUrl(QUrl(filePath,QUrl::TolerantMode));
}
