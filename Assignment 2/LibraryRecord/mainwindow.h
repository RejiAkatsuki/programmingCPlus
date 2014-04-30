#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnExit_clicked();

    void on_btnLibrary_clicked();

    void on_btnBorrower_clicked();

    void on_btnReport_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
