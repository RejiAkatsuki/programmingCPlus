#ifndef LIBRARY_H
#define LIBRARY_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QtSql>
namespace Ui {
class Library;
}

class Library : public QMainWindow
{
    Q_OBJECT

public:
    explicit Library(QWidget *parent = 0);
    ~Library();
    bool checkHide;
    bool checkEdit;
    bool checkImage;
    int selected_id;
    QModelIndex selected_index;
    static int collection_id;

private slots:
    void on_btnBack_clicked();

    void on_btnUpdate_clicked();

    void waitForAnimation();

    void fadeIn(QWidget *q);

    void fadeOut(QWidget *q);

    void on_btnAdd_clicked();

    void on_btnCancel_clicked();

    void readData();

    void on_btnCopy_clicked();

    void onClick(QModelIndex index);

    void clearText();

    void on_btnPicture_clicked();

    void on_btnFilter_clicked();

private:
    Ui::Library *ui;
    //this is a test model , must used SQL data model

    QSqlRelationalTableModel *queryModel;
};

#endif // LIBRARY_H
