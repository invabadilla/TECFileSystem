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
    manager->sendJson("find");
}

void MainWindow::on_actionBuscar_triggered()
{
    ServerManager *manager = ServerManager::getInstance();
    manager->sendJson("find");
}
