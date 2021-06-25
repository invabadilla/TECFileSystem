#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "List.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionAbrir_triggered();

    void on_pushButton_clicked();

    void on_tableWidget_cellClicked(int row, int column);

    void add_row(List<string> similars);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
