#include "borrowergui.h"
#include "ui_borrower.h"
#include "mainwindow.h"
#include <QRect>
#include <QDesktopWidget>
#include <QGraphicsEffect>
#include <QPropertyAnimation>
#include <QtSql>


BorrowerGui::BorrowerGui(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BorrowerGui)
{
    ui->setupUi(this);

    readData();

    checkEdit = false;

    //made the widget hide
    ui->widget->hide();
    checkHide = false;

    //set middle position for display screen
    QRect position = frameGeometry();
    position.moveCenter(QDesktopWidget().availableGeometry().center());
    move(position.topLeft());

    //set combo box for type
    ui->cbBrType->addItem(tr("student"));
    ui->cbBrType->addItem(tr("Teacher"));
    ui->cbBrType->addItem(tr("Etc"));

    //set combo box for department
    ui->cbBrDepartment->addItem(tr("IT"));
    ui->cbBrDepartment->addItem(tr("BCOM"));
    ui->cbBrDepartment->addItem(tr("FINANCE"));
    ui->cbBrDepartment->addItem(tr("ENGINEERING"));
    ui->cbBrDepartment->addItem(tr("Etc"));

}

BorrowerGui::~BorrowerGui()
{
    delete ui;
    //db.close();
}

void BorrowerGui::readData()
{
    queryModel = new QSqlTableModel();

    queryModel->setTable("borrowers");
    //queryModel->setFilter("collection_id="+collection);

    queryModel->select();

    queryModel->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    queryModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Name"));
    queryModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Mobile Number"));
    queryModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Type"));
    queryModel->setHeaderData(4, Qt::Horizontal, QObject::tr("Department"));

    queryModel->setHeaderData(5, Qt::Horizontal, QObject::tr("No.Item Borrowed"));
    queryModel->setHeaderData(6, Qt::Horizontal, QObject::tr("No.Item Late"));
    //select entire row
    ui->tbBorrower->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tbBorrower->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->tbBorrower->setModel(queryModel);
    ui->tbBorrower->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tbBorrower->setSortingEnabled(true);
    connect(ui->tbBorrower, SIGNAL(clicked(QModelIndex)),this, SLOT(onClick(QModelIndex)));

    ui->tbBorrower->show();


}

void BorrowerGui::on_btnBack_clicked()
{
    MainWindow *main = new MainWindow();
    main->show();
    this->hide();
}

void BorrowerGui::on_btnSave_clicked()
{
    clearText();
    checkEdit = false;
    if(checkHide == false){
      ui->widget->show();
      fadeIn();
      checkHide = true;
    }else if(checkHide == true){
      fadeOut();
      checkHide = false;
    }
    //do update to database from here

}

void BorrowerGui::fadeIn(){
    QGraphicsOpacityEffect * ophacityEffect = new QGraphicsOpacityEffect();
    ui->widget->setGraphicsEffect(ophacityEffect);
    QPropertyAnimation *anime = new QPropertyAnimation(ophacityEffect, "opacity");
    anime->setDuration(200);
    anime->setStartValue(0.0);
    anime->setEndValue(1.0);
    anime->setEasingCurve(QEasingCurve::OutQuad);
    anime->start(QAbstractAnimation::DeleteWhenStopped);
}

void BorrowerGui::fadeOut(){
    QGraphicsOpacityEffect * ophacityEffect = new QGraphicsOpacityEffect();
    ui->widget->setGraphicsEffect(ophacityEffect);

    QPropertyAnimation *anime = new QPropertyAnimation(ophacityEffect, "opacity");
    anime->setDuration(200);
    anime->setStartValue(1.0);
    anime->setEndValue(0.0);
    anime->setEasingCurve(QEasingCurve::OutQuad);
    anime->start(QAbstractAnimation::DeleteWhenStopped);
    connect(anime, SIGNAL(finished()), SLOT(waitForAnimation()));
}

void BorrowerGui::waitForAnimation(){
    ui->widget->hide();
}

void BorrowerGui::on_btnAdd_clicked()
{
    qDebug()<<checkEdit;
    if(checkEdit == false){
        //Add new borrower here
        QString name = ui->lnBrName->text();
        QString phone = ui->lnBrPhone->text();
        QString type = ui->cbBrType->currentText();
        QString department = ui->cbBrDepartment->currentText();


        //  qDebug( "Connected!" );

          QSqlQuery qry;

          qry.prepare( "CREATE TABLE IF NOT EXISTS borrowers (id INTEGER UNIQUE PRIMARY KEY , name VARCHAR(30), phone VARCHAR(30), type VARCHAR(30), department VARCHAR(100), items_borrowed INTEGER, items_late INTEGER )" );
          if( !qry.exec() )
            qDebug() << qry.lastError();
          else
            qDebug() << "Table created!";



          qry.prepare( "INSERT INTO borrowers (name, phone, type, department, items_borrowed, items_late) VALUES (:name,:phone,:type,:department,0,0)" );
          qry.bindValue(":name",name);
          qry.bindValue(":phone",phone);
          qry.bindValue(":type",type);
          qry.bindValue(":department",department);

          if( !qry.exec() )
            qDebug() << qry.lastError();
          else
            qDebug( "Inserted!" );

    } else {
        //Edit
    }



    //db.close();
    //fade out button
    fadeOut();
    checkHide=false;
    readData();

}

void BorrowerGui::on_btnCancel_clicked()
{
    //fade out button
    fadeOut();
    checkHide=false;
}

void BorrowerGui::onClick(QModelIndex index){
    qDebug()<<index;
    qDebug()<<"Collumn"<<index.column();
    qDebug()<<"Row"<<index.row();
    QModelIndex test = queryModel->index(index.row(), 0, QModelIndex());
    selected_index = test;
    qDebug()<<test.data();

    selected_id = test.data().toInt();
    qDebug()<< selected_id;
}

void BorrowerGui::clearText(){
    ui->lnBrName->clear();
    ui->lnBrPhone->clear();
}

void BorrowerGui::on_btnOverdue_clicked()
{
    if (ui->btnOverdue->text().compare("Show Overdue")==0) {
    queryModel = new QSqlTableModel();
    QSqlQueryModel *q = new QSqlQueryModel();
    queryModel->setTable("borrowers");
    queryModel->setFilter("items_late > 0");

    q->setQuery("SELECT * FROM borrowers WHERE id IN (select borrowed_by from copy where (julianday('now')-julianday(borrow_date) > days_borrowed))");
    q->query();
    queryModel->select();

    queryModel->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    queryModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Name"));
    queryModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Mobile Number"));
    queryModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Type"));
    queryModel->setHeaderData(4, Qt::Horizontal, QObject::tr("Department"));

    queryModel->setHeaderData(5, Qt::Horizontal, QObject::tr("No.Item Borrowed"));
    queryModel->setHeaderData(6, Qt::Horizontal, QObject::tr("No.Item Late"));
    //select entire row
    ui->tbBorrower->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tbBorrower->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->tbBorrower->setModel(q);
    ui->tbBorrower->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tbBorrower->setSortingEnabled(true);
    connect(ui->tbBorrower, SIGNAL(clicked(QModelIndex)),this, SLOT(onClick(QModelIndex)));

    ui->tbBorrower->show();

    ui->btnOverdue->setText("Show All");
    } else {
        readData();
        ui->btnOverdue->setText("Show Overdue");
    }
}
