#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qmessagebox.h"
#include "QFileDialog"
#include "QFile"
#include "QTextStream"
#include <iostream>
#include <filesystem>
#include "Divider.h"
#include "Client.h"
#include "json.hpp"
#include "Huffman.h"
#include <fstream>
#include <sstream>

using json = nlohmann::json;
using namespace filesystem;
using namespace std;

json parseJson (List<string> toSend, string key){
    json mymessage =
            {
                    {"key", key},
                    {"message", toSend.find(0)->getValue()},
                    {"pre", toSend.find(1)->getValue()},
                    {"in", toSend.find(2)->getValue()},

            };
    return mymessage;
}
List<string> StoL (string text, char char_){
    string insert;
    List<string> result;
    for (int i = 0; i < text.size(); i++) {
        if (text[i]==char_){
            result.insertLast(insert);
            insert ="";
        }
        else{
            insert.push_back(text[i]);
        }
    }
    if (char_ == '$' || char_ == '%'){
        return result;

    }
    result.insertLast(insert);
    return result;
}


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
    for(auto& p_: directory_iterator(fileName.toStdString())){;
        List<string> list = buildHuffmanTree(p_.path());
        json js = parseJson(list, "save");
        cout<<js.dump()<<endl;
        Client *client = Client::getInstance(56000);

        string messageR = client->sendJson(js.dump());
        cout<< messageR<<endl;
        json jmessageR = json::parse(messageR);

        string message = jmessageR.value("message", "oops");
        string pre = jmessageR.value("pre", "oops");
        string in = jmessageR.value("in", "oops");

        cout<<"message: "<<message<<endl;
        cout<<"pre: "<<pre<<endl;
        cout<<"in: "<<in<<endl;

        List<string> pre_list = StoL(pre, '$');
        List<string> in_list = StoL(in, '$');
        preIndex = 0;
        Tree_Node *root_ = buildTree(in_list, pre_list, 0, in_list.getSize()-1);

        int index = -1;
        string strDecode;
        while (index < (int)message.size() - 2) {
            decode(root_, index, message, &strDecode);
        }
        if (strDecode == "Error"){
            insertStatus = false;
        }
    }
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
    //ServerManager *manager = ServerManager::getInstance();
    QString tmp = ui->lineEdit->text();
    List<string> list = buildHuffmanTree(tmp.toStdString());
    json js = parseJson(list, "search");
    cout<<js.dump()<<endl;
    Client *client = Client::getInstance(56000);

    string messageR = client->sendJson(js.dump());
    cout<< messageR<<endl;
    json jmessageR = json::parse(messageR);

    string message = jmessageR.value("message", "oops");
    string pre = jmessageR.value("pre", "oops");
    string in = jmessageR.value("in", "oops");

    cout<<"message: "<<message<<endl;
    cout<<"pre: "<<pre<<endl;
    cout<<"in: "<<in<<endl;

    List<string> pre_list = StoL(pre, '$');
    List<string> in_list = StoL(in, '$');
    preIndex = 0;
    Tree_Node *root_ = buildTree(in_list, pre_list, 0, in_list.getSize()-1);

    int index = -1;
    string strDecode;
    while (index < (int)message.size() - 2) {
        decode(root_, index, message, &strDecode);
    }
    cout<<"message: "<<strDecode<<endl;
    if (strDecode == "Error404"){
        ui->tableWidget->setRowCount(0);
        ui->label->setText("No se han encontrado libros con ese nombre");
    }
    else{
        List<string> similars = StoL(strDecode, '%');
        add_row(similars);
         ui->label->setText("Se han encontrado libros con ese nombre");
    }
}

string convert(string input_){
    ifstream input(input_, ios::binary);
    vector<char> bytes(
            (istreambuf_iterator<char>(input)),
            (istreambuf_iterator<char>()));
    input.close();

    string texto;
    for (int i = 0; i < bytes.size(); i++) {
        texto+=bytes.at(i);
    }
    return texto;

}

string readData(string path, string name, string j){

    divider divider;

    int numError = -1;
    string firstPath;
    string firstBin;
    string secondPath;
    string secondBin;
    string thirdPath;
    string thirdBin;
    string fourthPath;
    string fourthBin;

    int i;
    for (i = 0; i < 3; ++i) {
        string pathToRead = path+to_string(i)+"/Block"+j+"/"+name+to_string(i)+".dat";
        ifstream my_file(pathToRead);
        if (my_file.good())
        {
            switch(i){
                case 0:
                    firstPath = pathToRead;
                    firstBin = convert(pathToRead);
                    break;
                case 1:
                    secondPath = pathToRead;
                    secondBin = convert(pathToRead);
                    break;
                case 2:
                    thirdPath = pathToRead;
                    thirdBin = convert(pathToRead);
                    break;
                default:
                    break;
            }

        }
        else{
            cout<<"El "<<i<<" no existe"<<endl;
            numError = i;
        }
    }
    fourthPath = path+to_string(3)+"/Block"+j+"/"+name+to_string(3)+".dat";
    fourthBin = convert(fourthPath);


    string convertido = "";
    if(numError == -1){
        cout<<"Resultado: ";
        convertido += divider.readData(firstPath);
        convertido.pop_back();
        convertido += divider.readData(secondPath);
        convertido.pop_back();
        convertido += divider.readData(thirdPath);
        convertido.pop_back();
        return convertido;
    }else{
        string paridad;
        switch(numError){
            case 0:
                paridad = divider.XoR(secondBin, thirdBin);
                paridad = divider.XoR(paridad, fourthBin);
                cout<<"Mensaje recuperado: ";
                convertido += divider.BinToS(paridad);
                convertido.pop_back();
                convertido += divider.readData(secondPath);
                convertido.pop_back();
                convertido += divider.readData(thirdPath);
                convertido.pop_back();
                return convertido;
                break;
            case 1:
                paridad = divider.XoR(firstBin, thirdBin);
                paridad = divider.XoR(paridad, fourthBin);
                cout<<"Mensaje recuperado: ";
                convertido += divider.readData(firstPath);
                convertido.pop_back();
                convertido += divider.BinToS(paridad);
                convertido.pop_back();
                convertido += divider.readData(thirdPath);
                convertido.pop_back();
                return convertido;
                break;
            case 2:
                paridad = divider.XoR(firstBin, secondBin);
                paridad = divider.XoR(paridad, fourthBin);
                cout<<"Mensaje recuperado: ";
                convertido += divider.readData(firstPath);
                convertido.pop_back();
                convertido += divider.readData(secondPath);
                convertido.pop_back();
                convertido += divider.BinToS(paridad);
                convertido.pop_back();
                return convertido;
                break;
            default:
                break;
        }
    }
}

void MainWindow::on_tableWidget_cellClicked(int row, int column)
{
    string name = ui->tableWidget->item(row, column)->text().toStdString();
    QString tmp = ui->lineEdit->text();
    List<string> list = buildHuffmanTree(name);
    json js = parseJson(list, "read");
    cout<<js.dump()<<endl;
    Client *client = Client::getInstance(56000);

    string messageR = client->sendJson(js.dump());
    cout<< messageR<<endl;
    json jmessageR = json::parse(messageR);

    string message = jmessageR.value("message", "oops");
    string pre = jmessageR.value("pre", "oops");
    string in = jmessageR.value("in", "oops");

    cout<<"message: "<<message<<endl;
    cout<<"pre: "<<pre<<endl;
    cout<<"in: "<<in<<endl;

    List<string> pre_list = StoL(pre, '$');
    List<string> in_list = StoL(in, '$');
    preIndex = 0;
    Tree_Node *root_ = buildTree(in_list, pre_list, 0, in_list.getSize()-1);

    int index = -1;
    string strDecode;
    while (index < (int)message.size() - 2) {
        decode(root_, index, message, &strDecode);
    }
    cout<<"message: "<<strDecode<<endl;

    List<string> for_read = StoL(strDecode, '#');

    string convertido = readData(for_read.find(0)->getValue(), name, for_read.find(1)->getValue());

    cout<<convertido<<endl;
    ui->textEdit->setText(QString::fromStdString(convertido));

}

/*!
 * \brief add a new row with string data
 *
 * \param dir string with the memory direction of the variable
 * \param label string with the label that identifies the variable
 * \param value string with the value of the variable
 * \param count string with the number of the pointers assigned to each memory location
 */
void MainWindow::add_row(List<string> similars)
{
    ui->tableWidget->setRowCount(0);
    for (int i = 0; i < similars.getSize(); ++i) {
        ui->tableWidget->insertRow(i);
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(similars.find(i)->getValue())));
    }

}

///*!
// * \brief deletes all the rows
// *
// */
//void MainWindow::delete_row()
//{
//    ui->tableWidget->setRowCount(0);
//}
