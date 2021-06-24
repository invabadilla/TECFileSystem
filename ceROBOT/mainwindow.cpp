#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qmessagebox.h"
#include "QFileDialog"
#include "QFile"
#include "QTextStream"
#include <iostream>
#include "ServerManager.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionAbrir_triggered()
{
    QString fileName;
    bool insertStatus = true;
    fileName = QFileDialog::getExistingDirectory(this, "Abrir");

    cout << fileName.toStdString() << endl;

    // insertar metodo que inserte todos los libros en el RAID

    if (insertStatus){
        ui->label->setText("Se han insertado los libros exitosamente!");
    } else {
        ui->label->setText("Error al insertar los libros.");
    }
}

void MainWindow::on_pushButton_clicked()
{
    // insertar metodo para las busquedas
    ServerManager *manager = ServerManager::getInstance();
    QString tmp = ui->lineEdit->text();
    manager->sendJson(tmp.toStdString());
}



void MainWindow::on_tableWidget_cellClicked(int row, int column)
{
    // accion de abrir el archivo selecionado
}

///*!
// * \brief add a new row with string data
// *
// * \param dir string with the memory direction of the variable
// * \param label string with the label that identifies the variable
// * \param value string with the value of the variable
// * \param count string with the number of the pointers assigned to each memory location
// */
//void MainWindow::add_row(string dir, string label, string value, string count)
//{
//    int row = ui->tableWidget->rowCount();
//    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
//    ui->tableWidget->setItem(row, DIR, new QTableWidgetItem(QString::fromStdString(dir)));
//    ui->tableWidget->setItem(row, LABEL, new QTableWidgetItem(QString::fromStdString(label)));
//    ui->tableWidget->setItem(row, VALUE, new QTableWidgetItem(QString::fromStdString(value)));
//    ui->tableWidget->setItem(row, COUNT, new QTableWidgetItem(QString::fromStdString(count)));
//}

///*!
// * \brief deletes all the rows
// *
// */
//void MainWindow::delete_row()
//{
//    ui->tableWidget->setRowCount(0);
//}
