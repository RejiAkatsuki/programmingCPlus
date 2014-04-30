#include "copygui.h"
#include "ui_copygui.h"
#include "library.h"
#include <QRect>
#include <QDesktopWidget>
#include <QGraphicsEffect>
#include <QPropertyAnimation>
#include <QtSql>
#include <QMessageBox>

CopyGui::CopyGui(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CopyGui)
{
    ui->setupUi(this);

    checkEdit = false;

    //combo box borrowed status
    ui->cbCPStatus->addItem(tr("In Storeroom"));
    ui->cbCPStatus->addItem(tr("In Library"));
    ui->cbCPStatus->addItem(tr("Lost"));
    ui->cbCPStatus->addItem(tr("Borrowed"));    
    ui->cbStatus->addItem(tr("All"));
    ui->cbStatus->addItem(tr("In Storeroom"));
    ui->cbStatus->addItem(tr("In Library"));
    ui->cbStatus->addItem(tr("Lost"));
    ui->cbStatus->addItem(tr("Borrowed"));


    //selected_id = Library::collection_id;
    readData();
    //made the widget hide
    ui->widget->hide();
    checkHide = false;

    //hide select borrower
    ui->widget_2->hide();
    checkHide2 = false;

    //set middle position for display screen
    QRect position = frameGeometry();
    position.moveCenter(QDesktopWidget().availableGeometry().center());
    move(position.topLeft());

    //Get borrower list
    QSqlQuery qry;
    qry.prepare( "SELECT * FROM borrowers");
    if( !qry.exec() )
        qDebug() << qry.lastError();
    else
    {
        for(int row = 0; qry.next(); row++){
            ui->cbBorrower->addItem(qry.value("name").toString(),qry.value("id").toInt());



//            for(int col = 0; col < cols; col++){

//                copyModel->setData(index, qry.value(col));//index of the table, data
//            }
        }
    }

}

CopyGui::~CopyGui()
{
    delete ui;
}

void CopyGui::fadeIn(QWidget *q){
    QGraphicsOpacityEffect * ophacityEffect = new QGraphicsOpacityEffect();
    q->setGraphicsEffect(ophacityEffect);
    QPropertyAnimation *anime = new QPropertyAnimation(ophacityEffect, "opacity");
    anime->setDuration(200);
    anime->setStartValue(0.0);
    anime->setEndValue(1.0);
    anime->setEasingCurve(QEasingCurve::OutQuad);
    anime->start(QAbstractAnimation::DeleteWhenStopped);
}

void CopyGui::fadeOut(QWidget *q){
    QGraphicsOpacityEffect * ophacityEffect = new QGraphicsOpacityEffect();
    q->setGraphicsEffect(ophacityEffect);
    QPropertyAnimation *anime = new QPropertyAnimation(ophacityEffect, "opacity");
    anime->setDuration(200);
    anime->setStartValue(1.0);
    anime->setEndValue(0.0);
    anime->setEasingCurve(QEasingCurve::OutQuad);
    anime->start(QAbstractAnimation::DeleteWhenStopped);
    connect(anime, SIGNAL(finished()), SLOT(waitForAnimation()));
}

void CopyGui::waitForAnimation(){
    ui->widget->hide();
    ui->widget_2->hide();
}

void CopyGui::on_btnCPSave_clicked()
{
    clearText();
    checkEdit = false;
    ui->btnCPAdd->setText("Add");
    if(checkHide == false){
      ui->widget->show();
      fadeIn(ui->widget);
      checkHide = true;
    }else if(checkHide == true){
      fadeOut(ui->widget);
      checkHide = false;
    }
}

void CopyGui::on_btnBack_clicked()
{
    Library *lib = new Library();
    lib->show();
    this->hide();
}

void CopyGui::on_btnCPAdd_clicked()
{
    //Add new collection to database
    QString barcode = ui->lnCPBarCode->text();
    QString status = ui->cbCPStatus->currentText();
    QString times_borrow = ui->lnCPTimeBorrowed->text();
    QString borrow_date = ui->dateBorrow->date().toString("yyyy-MM-dd");
    QString days_borrowed = ui->lnCPTimeBorrow->text();

    QSqlQuery qry;
    if (!checkEdit) {
        qry.prepare( "CREATE TABLE IF NOT EXISTS copy (id INTEGER UNIQUE PRIMARY KEY , barcode INTEGER UNIQUE, status VARCHAR(30), times_borrow INTEGER, borrow_date VARCHAR(30), days_borrowed INTEGER, collection_id INTEGER,borrowed_by INTEGER )" );
        if( !qry.exec() )
        qDebug() << qry.lastError();
        else
        qDebug() << "Table created!";


        qry.prepare( "INSERT INTO copy (barcode, status, times_borrow, days_borrowed, collection_id) VALUES (:barcode, :status, :times_borrow, :days_borrowed, :collection_id)" );
        qry.bindValue(":barcode",barcode);
        qry.bindValue(":status",status);
        qry.bindValue(":times_borrow",times_borrow);
        qry.bindValue(":borrow_date",borrow_date);

        qry.bindValue(":collection_id",selected_id);
        if( !qry.exec() )
        qDebug() << "error on inserted" <<qry.lastError();
        else
        qDebug( "Inserted!" );
    } else {
        int copy_id =queryModel->index(ui->tableView->currentIndex().row(),0,QModelIndex()).data().toInt();
        qry.prepare("UPDATE copy SET barcode=:barcode, status=:status, times_borrow=:times_borrow, borrow_date=:borrow_date, days_borrowed=:days_borrowed WHERE id=:id");
        qry.bindValue(":id",copy_id);
        qry.bindValue(":barcode",barcode);
        qry.bindValue(":status",status);
        qry.bindValue(":times_borrow",times_borrow);
        qry.bindValue(":borrow_date",borrow_date);
        qry.bindValue(":days_borrowed",days_borrowed);

        if( !qry.exec() )
        qDebug() << "error on edit copy" <<qry.lastError();
        else
        qDebug( "Edited!" );
    }


    //Fade out the add form
    fadeOut(ui->widget);
    checkHide = false;
    readData();
}

void CopyGui::on_btnCPCancel_clicked()
{
    //fade out button
    fadeOut(ui->widget);
    checkHide=false;
}

void CopyGui::onClick(QModelIndex index){
    qDebug()<<index;
    qDebug()<<"Collumn"<<index.column();
    qDebug()<<"Row"<<index.row();
    /*QModelIndex test = copyModel->index(index.row(), 0, QModelIndex());
    selected_index = test;
    qDebug()<<test.data();

    selected_id = test.data().toInt();
    qDebug()<< selected_id;*/
}

void CopyGui::clearText(){
    ui->lnCPBarCode->clear();    
    ui->lnCPTimeBorrow->clear();
    ui->lnCPTimeBorrowed->clear();
}

void CopyGui::on_btnEdit_clicked()
{
    checkEdit = true;
    QModelIndex modelBarcode = queryModel->index(ui->tableView->currentIndex().row(), 1, QModelIndex());
    QModelIndex modelBorrowingStatus = queryModel->index(ui->tableView->currentIndex().row(), 2, QModelIndex());
    QModelIndex modelTimeBorrowed = queryModel->index(ui->tableView->currentIndex().row(), 3, QModelIndex());
    QModelIndex modelDate = queryModel->index(ui->tableView->currentIndex().row(), 4, QModelIndex());
    QModelIndex modelTimeBorrow = queryModel->index(ui->tableView->currentIndex().row(), 5, QModelIndex());

    ui->lnCPBarCode->setText(modelBarcode.data().toString());
    ui->cbCPStatus->setCurrentText(modelBorrowingStatus.data().toString());
    ui->lnCPTimeBorrowed->setText(modelTimeBorrowed.data().toString());
    //ui->lnCPDateBorrow->setText(modelDate.data().toString());
    ui->dateBorrow->setDate(QDate::fromString(modelDate.data().toString(), "yyyy-MM-dd"));
    ui->lnCPTimeBorrow->setText(modelTimeBorrow.data().toString());

    ui->btnCPAdd->setText("Edit");

    if(checkHide == false){
      ui->widget->show();
      fadeIn(ui->widget);
      checkHide = true;
    }else if(checkHide == true){
      fadeOut(ui->widget);
      checkHide = false;
    }
}

void CopyGui::on_btnReturn_clicked()
{

    QModelIndex current_index_status = queryModel->index(ui->tableView->currentIndex().row(),2,QModelIndex());
    QSqlQuery qry;
    if (current_index_status.data().toString().compare("borrowed",Qt::CaseInsensitive)==0)
    {
        int copy_id = queryModel->index(ui->tableView->currentIndex().row(),0,QModelIndex()).data().toInt();
        int borrower_id = queryModel->index(ui->tableView->currentIndex().row(),7,QModelIndex()).data().toInt();

        QDate borrow_date = QDate::fromString(queryModel->index(ui->tableView->currentIndex().row(),4,QModelIndex()).data().toString(),"yyyy-MM-dd");
        int days_borrow = queryModel->index(ui->tableView->currentIndex().row(),5,QModelIndex()).data().toInt();
        QDate due_date = borrow_date.addDays(days_borrow);

        QDate today = QDate::currentDate();
        int days_late = due_date.daysTo(today);

        qDebug() << "borrow Date:" <<borrow_date << " - days borrow:" << days_borrow << " - due date:" <<due_date <<" - late:" << days_late;
        qry.prepare("UPDATE copy SET status='In Library', days_borrowed = 0, borrow_date = null, borrowed_by = null WHERE id=:copy_id ");
        qry.bindValue(":copy_id",copy_id);
        if( !qry.exec() )
            qDebug() << "update copy return fail: " <<qry.lastError();

        if (days_late > 0) {
            qry.prepare( "UPDATE borrowers SET items_late = items_late +1 WHERE id=:id ");
            qry.bindValue(":id", borrower_id);
            if( !qry.exec() )
                qDebug() << "update borrower late return fail: " <<qry.lastError();
            QMessageBox::information(0, QString("Notice"), QString("This copy is return late for "+QString::number(days_late)+" days!!"), QMessageBox::Ok);
        } else {
            QMessageBox::information(0, QString("Notice"), QString("This copy is return successfully!!"), QMessageBox::Ok);
        }


    } else {
        QMessageBox::information(0, QString("Notice"), QString("This copy is not borrowed by anyone."), QMessageBox::Ok);
    }

    readData();
}

void CopyGui::on_btnBorrow_clicked()
{
    if(checkHide2 == false){
        QModelIndex current_index_status = queryModel->index(ui->tableView->currentIndex().row(),2,QModelIndex());
        if (current_index_status.data().toString().compare("lost",Qt::CaseInsensitive)==0) {
            QMessageBox::information(0, QString("Notice"), QString("This copy is lost."), QMessageBox::Ok);
        }
        else if (current_index_status.data().toString().compare("borrowed",Qt::CaseInsensitive)!=0)
        {
            ui->widget_2->show();
            fadeIn(ui->widget_2);
            checkHide2 = true;
        } else {
            QMessageBox::information(0, QString("Notice"), QString("This copy is already borrowed by someone."), QMessageBox::Ok);
        }


    }else if(checkHide2 == true){
      fadeOut(ui->widget_2);
      checkHide2 = false;
    }
}

void CopyGui::on_btnCancelBorrower_clicked()
{
    //fade out button
        fadeOut(ui->widget_2);
        checkHide2=false;
}

void CopyGui::on_btnSelectBorrower_clicked()
{

    QModelIndex current_index = ui->tableView->currentIndex();
    QModelIndex current_index_id = queryModel->index(current_index.row(), 0, QModelIndex());
    QModelIndex current_index_status = queryModel->index(current_index.row(),2,QModelIndex());

    int days_borrow = ui->spinDaysBorrow->text().toInt();


    qDebug() << current_index_id.data().toInt() << " - "<< current_index_status.data().toString();
    int copy_id = current_index_id.data().toInt();
    int borrower_id = ui->cbBorrower->itemData(ui->cbBorrower->currentIndex()).toInt();
    QSqlQuery qry;
    qry.prepare( "UPDATE borrowers SET items_borrowed = items_borrowed +1 WHERE id=:id ");
    qry.bindValue(":id", borrower_id);
    if( !qry.exec() )
        qDebug() << "update borrower fail: " <<qry.lastError();

    qry.prepare("UPDATE copy SET status='Borrowed', times_borrow = times_borrow + 1, days_borrowed = :days_borrow, borrow_date = date('now'), borrowed_by = :borrower_id WHERE id=:copy_id ");
    qry.bindValue(":days_borrow",days_borrow);
    qry.bindValue(":copy_id",copy_id);
    qry.bindValue(":borrower_id",borrower_id);
    if( !qry.exec() )
        qDebug() << "update copy fail "<< qry.lastError();

    fadeOut(ui->widget_2);
    checkHide2=false;
    readData();


}


void CopyGui::readData()
{
    QSqlQuery qry;
    qry.prepare( "SELECT * FROM collections WHERE id=:collection");
    qry.bindValue(":collection", selected_id);
    if( !qry.exec() )
        qDebug() << qry.lastError();
    else
    {
        qry.next();
        ui->lblHeader->setText("<html><head/><body><p align='center'><span style='font-size:14pt; color:#ff0000;'>Copies of "+qry.value("title").toString()+"</span></p></body></html>");
    }


    queryModel = new QSqlRelationalTableModel();
    QString collection = QString::number(selected_id);
    queryModel->setTable("copy");
    queryModel->setFilter("collection_id="+collection);
    queryModel->setRelation(6,QSqlRelation("collections","id","title"));
    queryModel->setRelation(7,QSqlRelation("borrowers","id","name"));
    queryModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    queryModel->setJoinMode(QSqlRelationalTableModel::LeftJoin);
    queryModel->select();

    queryModel->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    queryModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Barcode"));
    queryModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Borrowing Status"));
    queryModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Times Borrowed"));
    queryModel->setHeaderData(4, Qt::Horizontal, QObject::tr("Borrowed Date"));
    queryModel->setHeaderData(5, Qt::Horizontal, QObject::tr("Days currently borrower"));
    queryModel->setHeaderData(6, Qt::Horizontal, QObject::tr("Collection ID"));
    queryModel->setHeaderData(7, Qt::Horizontal, QObject::tr("Borrowed by"));


    //select entire row
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->tableView->setModel(queryModel);
    ui->tableView->setSortingEnabled(true);
    //connect(ui->tableView, SIGNAL(clicked(QModelIndex)),this, SLOT(onClick(QModelIndex)));
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->show();
}

void CopyGui::on_btnFilter_clicked()
{
    QString collection = QString::number(selected_id);
    QString sql = "collection_id="+collection;
    if (ui->cbStatus->currentText().compare("All")) {
        sql += " AND status LIKE '" + ui->cbStatus->currentText() + "'";
    }
    qDebug() << sql;

    queryModel = new QSqlRelationalTableModel();

    queryModel->setTable("copy");
    queryModel->setFilter(sql);
    queryModel->setRelation(6,QSqlRelation("collections","id","title"));
    queryModel->setRelation(7,QSqlRelation("borrowers","id","name"));
    queryModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    queryModel->setJoinMode(QSqlRelationalTableModel::LeftJoin);
    queryModel->select();

    queryModel->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    queryModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Barcode"));
    queryModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Borrowing Status"));
    queryModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Times Borrowed"));
    queryModel->setHeaderData(4, Qt::Horizontal, QObject::tr("Borrowed Date"));
    queryModel->setHeaderData(5, Qt::Horizontal, QObject::tr("Days currently borrower"));
    queryModel->setHeaderData(6, Qt::Horizontal, QObject::tr("Collection ID"));
    queryModel->setHeaderData(7, Qt::Horizontal, QObject::tr("Borrowed by"));


    //select entire row
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->tableView->setModel(queryModel);
    ui->tableView->setSortingEnabled(true);

    //connect(ui->tableView, SIGNAL(clicked(QModelIndex)),this, SLOT(onClick(QModelIndex)));
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->show();
}
