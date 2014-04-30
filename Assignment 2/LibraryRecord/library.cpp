#include "library.h"
#include "ui_library.h"
#include "mainwindow.h"
#include "collection_list.h"
#include <QRect>
#include <QDesktopWidget>
#include <QGraphicsEffect>
#include <QPropertyAnimation>
#include <QtSql>
#include <QDebug>
#include "copygui.h"
#include <QFileDialog>

CollectionList collections;

Library::Library(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Library)
{
    ui->setupUi(this);

    readData();
    // QTableView::selectionChanged(selected,deselected);
    //ui->tbCollection->selectionChanged(selected,selected);
    checkEdit = false;

    /*//hide picture display
    ui->widget_image->hide();
    checkImage = false;*/

    //hide add new collection
    ui->widget->hide();
    checkHide = false;

    //set middle position for display screen
    QRect position = frameGeometry();
    position.moveCenter(QDesktopWidget().availableGeometry().center());
    move(position.topLeft());

    //combo box type
    ui->cbCollType->addItem("CD");
    ui->cbCollType->addItem("DVD");
    ui->cbType->addItem("All");
    ui->cbType->addItem("CD");
    ui->cbType->addItem("DVD");

    //combo box data type
    ui->cbCollDataType->addItem("image");
    ui->cbCollDataType->addItem("video");
    ui->cbCollDataType->addItem("audio");
    ui->cbDataType->addItem("All");
    ui->cbDataType->addItem("image");
    ui->cbDataType->addItem("video");
    ui->cbDataType->addItem("audio");

    //combo box category
    ui->cbCollCategory->addItem("Engineering and Technology");
    ui->cbCollCategory->addItem("History");
    ui->cbCollCategory->addItem("Film and media studies");
    ui->cbCollCategory->addItem("Psychology");
    ui->cbCollCategory->addItem("Images");
    ui->cbCollCategory->addItem("Photoshop & Map");
    ui->cbCollCategory->addItem("Business and Economies");
    ui->cbCollCategory->addItem("General");

    ui->cbCategory->addItem("All");
    ui->cbCategory->addItem("Engineering and Technology");
    ui->cbCategory->addItem("History");
    ui->cbCategory->addItem("Film and media studies");
    ui->cbCategory->addItem("Psychology");
    ui->cbCategory->addItem("Images");
    ui->cbCategory->addItem("Photoshop & Map");
    ui->cbCategory->addItem("Business and Economies");
    ui->cbCategory->addItem("General");

    //combo box for year record
    for(int i = 1945; i <= 2013; i ++ ){
        QString year = QString::number(i);
        ui->cbYearRecord->addItem(year);
    }


}

Library::~Library()
{
    delete ui;

}

void Library::readData()
{
    queryModel = new QSqlRelationalTableModel();

    queryModel->setTable("collections");
    //queryModel->setFilter("collection_id="+collection);
    queryModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    queryModel->select();

    queryModel->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    queryModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Title"));
    queryModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Type"));
    queryModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Data Type"));
    queryModel->setHeaderData(4, Qt::Horizontal, QObject::tr("Version"));
    queryModel->setHeaderData(5, Qt::Horizontal, QObject::tr("Indentification code"));
    queryModel->setHeaderData(6, Qt::Horizontal, QObject::tr("Publisher"));
    queryModel->setHeaderData(7, Qt::Horizontal, QObject::tr("Year of Recorded"));
    queryModel->setHeaderData(8, Qt::Horizontal, QObject::tr("Duration"));
    queryModel->setHeaderData(9, Qt::Horizontal, QObject::tr("Category"));

    //select entire row
    ui->tbCollection->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tbCollection->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->tbCollection->setModel(queryModel);
    ui->tbCollection->setSortingEnabled(true);
    connect(ui->tbCollection, SIGNAL(clicked(QModelIndex)),this, SLOT(onClick(QModelIndex)));
    ui->tbCollection->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tbCollection->show();


}

void Library::on_btnBack_clicked()
{
    MainWindow *main = new MainWindow();
    main->show();
    this->hide();
}

void Library::on_btnUpdate_clicked()
{
    clearText();
    if(checkHide == false){
      ui->widget->show();
      //fade in
      fadeIn(ui->widget);
      checkHide = true;
    }else if(checkHide == true){
      //fade out
      fadeOut(ui->widget);
      checkHide = false;
    }
    //update data and push to server here if data is not blank
}

void Library::fadeIn(QWidget *q){
    QGraphicsOpacityEffect * ophacityEffect = new QGraphicsOpacityEffect();
    q->setGraphicsEffect(ophacityEffect);
    QPropertyAnimation *anime = new QPropertyAnimation(ophacityEffect, "opacity");
    anime->setDuration(200);
    anime->setStartValue(0.0);
    anime->setEndValue(1.0);
    anime->setEasingCurve(QEasingCurve::OutQuad);
    anime->start(QAbstractAnimation::DeleteWhenStopped);
}

void Library::fadeOut(QWidget *q){
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

void Library::waitForAnimation(){
    ui->widget->hide();
}

void Library::on_btnAdd_clicked()
{
    //Add new collection to database
    QString title = ui->lnCollTitle->text();
    QString type = ui->cbCollType->currentText();
    QString dataType = ui->cbCollDataType->currentText();
    QString version = ui->lnCollVersion->text();
    QString id_code = ui->lnCollIndenCode->text();
    QString publisher = ui->lnCollPublisher->text();
    QString year = ui->cbYearRecord->currentText();
    QString duration = ui->lnCollDuration->text();
    QString category = ui->cbCollCategory->currentText();

    QSqlQuery qry;

    qry.prepare( "CREATE TABLE IF NOT EXISTS collections (id INTEGER UNIQUE PRIMARY KEY , title VARCHAR(30), type VARCHAR(30), datatype VARCHAR(30), version VARCHAR(30), idcode INTEGER UNIQUE, publisher VARCHAR(30), year INTEGER, duration VARCHAR(30),category VARCHAR(100) )" );
    if( !qry.exec() )
    qDebug() << qry.lastError();
    else
    qDebug() << "Table created!";


    qry.prepare( "INSERT INTO collections (title, type, datatype, version, idcode, publisher, year, duration, category) VALUES (:title,:type,:datatype,:version,:idcode,:publisher,:year,:duration,:category)" );
    qry.bindValue(":title",title);
    qry.bindValue(":type",type);
    qry.bindValue(":datatype",dataType);
    qry.bindValue(":version",version);
    qry.bindValue(":idcode",id_code);
    qry.bindValue(":publisher",publisher);
    qry.bindValue(":year",year);
    qry.bindValue(":duration",duration);
    qry.bindValue(":category",category);
    if( !qry.exec() )
    qDebug() << qry.lastError();
    else
    qDebug( "Inserted!" );

    //Fade out the add form
    fadeOut(ui->widget);
    checkHide = false;
    readData();
}

void Library::onClick(QModelIndex index){
    qDebug()<<index;
    qDebug()<<"Collumn"<<index.column();
    qDebug()<<"Row"<<index.row();
    QModelIndex test = queryModel->index(index.row(), 0, QModelIndex());
    selected_index = test;
    qDebug()<<test.data();

    selected_id = test.data().toInt();
    qDebug()<< selected_id;
    QPixmap pix ("./" + QString::number(selected_id));
    ui->lblImage->setScaledContents(true);
    ui->lblImage->setPixmap(pix);
}

void Library::on_btnCancel_clicked()
{
    fadeOut(ui->widget);
    checkHide = false;
}


void Library::on_btnCopy_clicked()
{
    CopyGui *cpGui = new CopyGui();
    cpGui->selected_id = selected_id;
    cpGui->selected_index = selected_index;
    cpGui->readData();
    cpGui->show();
    this->hide();
}

void Library::clearText(){
    ui->lnCollDuration->clear();
    ui->lnCollIndenCode->clear();
    ui->lnCollPublisher->clear();
    ui->lnCollTitle->clear();
    ui->lnCollVersion->clear();
}

void Library::on_btnPicture_clicked()
{
    //QString s = QFileDialog::getOpenFileName("./", "All Files (*.*)", this, "open file dialog", "Choose a file..." );
    //inputEdit->setText(s);
    QString pathFile = QFileDialog::getOpenFileName(this, tr("Open File"),"/path/to/file/");
    qDebug()<<pathFile;
    QFile fileName;
    fileName.setFileName("./" + QString::number(selected_id));
    if(fileName.exists()){
        fileName.remove();
    }
    QFile::copy(pathFile, "./" + QString::number(selected_id));
    QPixmap pix ("./" + QString::number(selected_id));
    ui->lblImage->setScaledContents(true);
    ui->lblImage->setPixmap(pix);
}

void Library::on_btnFilter_clicked()
{
    QString type = ui->cbType->currentText();
    QString dataType = ui->cbDataType->currentText();
    QString category = ui->cbCategory->currentText();
    bool haveFilter = false;
    bool haveAnd = false;
    QString sql = "";
    if (type.compare("All")||dataType.compare("All")||category.compare("All")) {
        haveFilter = true;
    }

    if (haveFilter) {

        if (type.compare("All")) {
            sql += "type LIKE '" + type + "'";
            haveAnd= true;
        }

        if (dataType.compare("All")) {
            if (haveAnd) {
                sql += " AND ";
            }
            sql += "datatype LIKE '" + dataType + "'";
            haveAnd= true;
        }

        if (category.compare("All")) {
            if (haveAnd) {
                sql += " AND ";
            }
            sql += "category LIKE '" + category + "'";
            haveAnd= true;
        }

    } else {
        sql += "1=1";
    }

    qDebug() << sql;

    queryModel = new QSqlRelationalTableModel();

    queryModel->setTable("collections");
    queryModel->setFilter(sql);
    queryModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    queryModel->select();

    queryModel->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    queryModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Title"));
    queryModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Type"));
    queryModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Data Type"));
    queryModel->setHeaderData(4, Qt::Horizontal, QObject::tr("Version"));
    queryModel->setHeaderData(5, Qt::Horizontal, QObject::tr("Indentification code"));
    queryModel->setHeaderData(6, Qt::Horizontal, QObject::tr("Publisher"));
    queryModel->setHeaderData(7, Qt::Horizontal, QObject::tr("Year of Recorded"));
    queryModel->setHeaderData(8, Qt::Horizontal, QObject::tr("Duration"));
    queryModel->setHeaderData(9, Qt::Horizontal, QObject::tr("Category"));
    //select entire row
    ui->tbCollection->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tbCollection->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->tbCollection->setModel(queryModel);
    ui->tbCollection->setSortingEnabled(true);
    connect(ui->tbCollection, SIGNAL(clicked(QModelIndex)),this, SLOT(onClick(QModelIndex)));
    ui->tbCollection->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tbCollection->show();

}
