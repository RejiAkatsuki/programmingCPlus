#ifndef BORROWERGUI_H
#define BORROWERGUI_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QtSql>

namespace Ui {
class BorrowerGui;
}

class BorrowerGui : public QMainWindow
{
    Q_OBJECT

public:
    explicit BorrowerGui(QWidget *parent = 0);
    ~BorrowerGui();
    bool checkHide;
    bool checkEdit;
    int selected_id;
    QModelIndex selected_index;

private slots:
    void on_btnBack_clicked();

    void on_btnSave_clicked();

    void waitForAnimation();

    void fadeIn();

    void fadeOut();

    void on_btnAdd_clicked();

    void on_btnCancel_clicked();

    void readData();

    void onClick(QModelIndex index);    

    void clearText();

    void on_btnOverdue_clicked();

private:
    Ui::BorrowerGui *ui;
    //this is a test model , must used SQL data model

    QSqlTableModel *queryModel;
};

#endif // BORROWER_H
