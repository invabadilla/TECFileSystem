#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qmessagebox.h"
#include "QFileDialog"
#include "QFile"
#include "QTextStream"
#include <iostream>

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
    QFile file;
    QTextStream io;
    QString fileName;
    fileName = QFileDialog::getExistingDirectory(this, "Abrir");

    cout << fileName.toStdString() << endl;
//    file.setFileName(fileName);
//    file.open(QIODevice::ReadOnly | QIODevice::Text);
//    if (!file.isOpen())
//    {
//        QMessageBox::critical(this, "Error", file.errorString());
//        return;
//    }
//    io.setDevice(&file);
//    ui->plainTextEdit->setPlainText(io.readAll());
//    file.flush();
//    file.close();
}
