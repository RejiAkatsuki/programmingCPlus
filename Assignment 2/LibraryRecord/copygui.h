#ifndef COPYGUI_H
#define COPYGUI_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QtSql>


namespace Ui {
class CopyGui;
}

class CopyGui : public QMainWindow
{
    Q_OBJECT

public:
    explicit CopyGui(QWidget *parent = 0);
    ~CopyGui();
    bool checkHide;
    bool checkHide2;
    bool checkEdit;
    int selected_id;
    QModelIndex selected_index;
    void readData();

private slots:
    void on_btnCPSave_clicked();

    void on_btnBack_clicked();

    void waitForAnimation();

    void fadeIn(QWidget *q);

    void fadeOut(QWidget *q);

    void on_btnCPAdd_clicked();

    void on_btnCPCancel_clicked();

    void onClick(QModelIndex index);

    void clearText();

    void on_btnEdit_clicked();

    void on_btnReturn_clicked();

    void on_btnBorrow_clicked();

    void on_btnCancelBorrower_clicked();

    void on_btnSelectBorrower_clicked();



    void on_btnFilter_clicked();

private:
    Ui::CopyGui *ui;

    QSqlRelationalTableModel *queryModel;
};

#endif // COPYGUI_H
