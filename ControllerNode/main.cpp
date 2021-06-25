#include <iostream>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <fstream>
#include <filesystem>
#include "functional"
#include "Divider.h"
#include "tinyxml2.h"
#include "Huffman.h"
#include "json.hpp"
#include "Client.h"
#include <string>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
using namespace tinyxml2;
using namespace filesystem;
using json = nlohmann::json;

int globalPort;
List<string> nameMD;




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
    if (char_ != '$'){
        result.insertLast(insert);
    }
    return result;
}

string LtoS (List<string> text){
    string result;
    for (int i = 0; i < text.getSize(); i++) {
        result += text.find(i)->getValue()+"%";
    }
    return result;
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

string requestSearch(string name){
    string texto = convert("/home/usuario/Proyectos/TECFileSystem/ControllerNode/METADATA.txt");
    List<string> names = StoL(texto, '%');
    List<string> nameResult;

    for (int i = 0; i < names.getSize(); ++i) {
        if(names.find(i)->getValue().size() >= name.size()){
            string temp;
            for (int j = 0; j < name.size(); ++j) {
                temp += names.find(i)->getValue()[j];
            }
            if(temp == name){nameResult.insertLast(names.find(i)->getValue());}
            else{continue;}
        }
        else{continue;}
    }
    if (nameResult.getSize() == 0){

        return  "Error404";
    }
    return LtoS(nameResult);

}


int StartListenign(int port){
    int listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == -1)
    {
        cerr << "No se pudo crear el Socket. Cerrando..." << endl;
        return -1;
    }

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);

    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);
    bind(listening, (sockaddr*)&hint, sizeof(hint));

    listen(listening, SOMAXCONN);

    while(true){
        sockaddr_in client;
        socklen_t clientSize = sizeof(client);

        int clientSocket = accept(listening, (sockaddr *) &client, &clientSize);

        char host[NI_MAXHOST];      // Nombre del cliente remoto
        char service[NI_MAXSERV];

        memset(host, 0, NI_MAXHOST);
        memset(service, 0, NI_MAXSERV);

        if (getnameinfo((sockaddr *) &client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
            cout << host << " conectado " << endl;
        } else {
            inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
            cout << host << " conectado al puerto " << ntohs(client.sin_port) << endl;
        }
        char buf[4096];
        memset(buf, 0, 4096);

        int bytesReceived = recv(clientSocket, buf, 4096, 0);
        if (bytesReceived == -1) {
            cerr << "Error al recibir mensaje. Cerrando..." << endl;
            break;
        }

        if (bytesReceived == 0) {
            cout << "Cliente desconectado " << endl;
            break;
        }

        string messageR = string(buf, 0, bytesReceived);
        json jmessageR = json::parse(messageR);
        string key = jmessageR.value("key", "oops");
        if (key == "save"){

            string message = jmessageR.value("message", "oops");
            string pre = jmessageR.value("pre", "oops");
            string in = jmessageR.value("in", "oops");

            cout<<"message: "<<message<<endl;
            cout<<"pre: "<<pre<<endl;
            cout<<"in: "<<in<<endl;

            List<string> pre_list = StoL(pre, '$');
            List<string> in_list = StoL(in, '$');
            preIndex = 0;
            Tree_Node *root = buildTree(in_list, pre_list, 0, in_list.getSize()-1);

            int index = -1;
            string strDecode;
            while (index < (int)message.size() - 2) {
                decode(root, index, message, &strDecode);
            }
            cout<<"message: "<<strDecode<<endl;
            string input_ = strDecode;
            string texto = convert(input_);

            divider divider;

            // Print 3 equal parts of the string
            divider.divideString(const_cast<char *>(texto.c_str()), 3, texto);
            List<string> result = StoL(input_, '/');
            string name = result.find(result.getSize()-1)->getValue();
            name.erase(name.end()-4, name.end());
            divider.createDatcopy("", name);

            string size_ = to_string(file_size(name+to_string(0)+".dat"));
            List<string> list = buildHuffmanTree("0"+size_);
            json js = parseJson(list, "save");
            cout<<js.dump()<<endl;
            Client *client_ = Client::getInstance(globalPort);

            string messageR_ = client_->sendJson(js.dump());
            json jmessageR_ = json::parse(messageR_);

            string message_ = jmessageR_.value("message", "oops");
            string pre_ = jmessageR_.value("pre", "oops");
            string in_ = jmessageR_.value("in", "oops");

            cout<<"message: "<<message_<<endl;
            cout<<"pre: "<<pre_<<endl;
            cout<<"in: "<<in_<<endl;

            List<string> pre_list_ = StoL(pre_, '$');
            List<string> in_list_ = StoL(in_, '$');
            preIndex = 0;
            Tree_Node *root_ = buildTree(in_list_, pre_list_, 0, in_list_.getSize()-1);

            int index_ = -1;
            string strDecode_;
            while (index_ < (int)message_.size() - 2) {
                decode(root_, index_, message_, &strDecode_);
            }
            cout<<"message: "<<strDecode_<<endl;
            List<string> create = StoL(strDecode_, '#');
            divider.createDat(create.find(0)->getValue(), name, create.find(1)->getValue());

            nameMD.insertLast(name);
            string metadata = LtoS(nameMD);

            std::fstream toWrite;
            toWrite.open("/home/usuario/Proyectos/TECFileSystem/ControllerNode/METADATA.txt", std::ios::out | std::ios::binary);
            toWrite.write(metadata.c_str(), metadata.size());
            //Ahora cierro el archivo
            toWrite.close();


            List<string> list_ = buildHuffmanTree("creado");
            json js_ = parseJson(list_, "save");
            string messageS = js_.dump();
            send(clientSocket, messageS.c_str(), messageS.size() + 1, 0);
        }
        if (key == "search"){

            string message = jmessageR.value("message", "oops");
            string pre = jmessageR.value("pre", "oops");
            string in = jmessageR.value("in", "oops");

            cout<<"message: "<<message<<endl;
            cout<<"pre: "<<pre<<endl;
            cout<<"in: "<<in<<endl;

            List<string> pre_list = StoL(pre, '$');
            List<string> in_list = StoL(in, '$');
            preIndex = 0;
            Tree_Node *root = buildTree(in_list, pre_list, 0, in_list.getSize()-1);

            int index = -1;
            string strDecode;
            while (index < (int)message.size() - 2) {
                decode(root, index, message, &strDecode);
            }
            cout<<"message: "<<strDecode<<endl;

            string similars = requestSearch(strDecode);
            cout<<"send: "<<similars<<endl;
            List<string> list_ = buildHuffmanTree(similars);
            json js_ = parseJson(list_, "search");
            string messageS = js_.dump();
            send(clientSocket, messageS.c_str(), messageS.size() + 1, 0);
        }

        if (key == "read")
        {
            string message = jmessageR.value("message", "oops");
            string pre = jmessageR.value("pre", "oops");
            string in = jmessageR.value("in", "oops");

            cout<<"message: "<<message<<endl;
            cout<<"pre: "<<pre<<endl;
            cout<<"in: "<<in<<endl;

            List<string> pre_list = StoL(pre, '$');
            List<string> in_list = StoL(in, '$');
            preIndex = 0;
            Tree_Node *root = buildTree(in_list, pre_list, 0, in_list.getSize()-1);

            int index = -1;
            string strDecode;
            while (index < (int)message.size() - 2) {
                decode(root, index, message, &strDecode);
            }
            cout<<"message: "<<strDecode<<endl;

            List<string> list = buildHuffmanTree(strDecode);
            json js = parseJson(list, "read");
            cout<<js.dump()<<endl;
            Client *client_ = Client::getInstance(globalPort);

            string messageR_ = client_->sendJson(js.dump());
            json jmessageR_ = json::parse(messageR_);

            string message_ = jmessageR_.value("message", "oops");
            string pre_ = jmessageR_.value("pre", "oops");
            string in_ = jmessageR_.value("in", "oops");

            cout<<"message: "<<message_<<endl;
            cout<<"pre: "<<pre_<<endl;
            cout<<"in: "<<in_<<endl;

            List<string> pre_list_ = StoL(pre_, '$');
            List<string> in_list_ = StoL(in_, '$');
            preIndex = 0;
            Tree_Node *root_ = buildTree(in_list_, pre_list_, 0, in_list_.getSize()-1);

            int index_ = -1;
            string strDecode_;
            while (index_ < (int)message_.size() - 2) {
                decode(root_, index_, message_, &strDecode_);
            }
            cout<<"message: "<<strDecode_<<endl;
            List<string> list_ = buildHuffmanTree(strDecode_);
            json js_ = parseJson(list_, "read");
            string messageS = js_.dump();
            send(clientSocket, messageS.c_str(), messageS.size() + 1, 0);
        }
        close(clientSocket);
    }

    return 0;
}


int main() {

//    //string input_ ="/home/usuario/Proyectos/TECFileSystem/ControllerNode/hola.txt";
//    string input_ = "/home/ingrid/Documents/TECFileSystem/ControllerNode/hola.txt";
//
//    ifstream input(input_, ios::binary);
//    vector<char> bytes(
//            (istreambuf_iterator<char>(input)),
//            (istreambuf_iterator<char>()));
//    input.close();
//
//    string texto;
//    for (int i = 0; i < bytes.size(); i++) {
//        texto+=bytes.at(i);
//    }
//
//    divider divider;
//
//    // Print 3 equal parts of the string
//    divider.divideString(const_cast<char *>(texto.c_str()), 3, texto);
//    List<string> result = StoL(input_, '/');
//    string name = result.find(result.getSize()-1)->getValue();
//    name.erase(name.end()-4, name.end());
//    divider.createDatcopy("", name);

    XMLDocument xml_doc;
    path temp = current_path().parent_path().parent_path();
    string dn =temp.string()+"/RAID/Parameters_TECFS_Disk.xml"; const char *document_name = dn.c_str();
    XMLError eResult = xml_doc.LoadFile(document_name);
    XMLNode* root = xml_doc.FirstChildElement("Parameters");
    XMLElement *port = root->FirstChildElement("port");
    globalPort = stoi(port->GetText());

    StartListenign(56000);

    //**********************************************************************//

//    remove("/home/ingrid/Documents/TECFileSystem/ControllerNode/cmake-build-debug/hola2.dat");
//
//
//    int numError = -1;
//    string firstPath;
//    string firstBin;
//    string secondPath;
//    string secondBin;
//    string thirdPath;
//    string thirdBin;
//    string fourthPath;
//    string fourthBin;
//    int i;
//    for (i = 0; i < 3; ++i) {
//        // /home/ingrid/Documents/TECFileSystem/RAID/DiskNodes/DiskNode+i+/Block+j/name+i.dat
//        string pathToRead = "/home/ingrid/Documents/TECFileSystem/ControllerNode/cmake-build-debug/hola" + to_string(i) + ".dat" ;
//        ifstream my_file(pathToRead);
//        if (my_file.good())
//        {
//            switch(i){
//                case 0:
//                    firstPath = pathToRead;
//                    firstBin = convert(pathToRead);
//                    break;
//                case 1:
//                    secondPath = pathToRead;
//                    secondBin = convert(pathToRead);
//                    break;
//                case 2:
//                    thirdPath = pathToRead;
//                    thirdBin = convert(pathToRead);
//                    break;
//                default:
//                    break;
//            }
//
//        }
//        else{
//            cout<<"El "<<i<<" no existe"<<endl;
//            numError = i;
//        }
//    }
//    fourthPath = "/home/ingrid/Documents/TECFileSystem/ControllerNode/cmake-build-debug/hola3.dat";
//    fourthBin = convert("/home/ingrid/Documents/TECFileSystem/ControllerNode/cmake-build-debug/hola3.dat");
//
//
//    string convertido = "";
//    if(numError == -1){
//        cout<<"Resultado: ";
//        convertido += divider.readData(firstPath);
//        convertido.pop_back();
//        convertido += divider.readData(secondPath);
//        convertido.pop_back();
//        convertido += divider.readData(thirdPath);
//        convertido.pop_back();
//        cout<<convertido<<endl;
//    }else{
//        string paridad;
//        switch(numError){
//            case 0:
//
//                paridad = divider.XoR(secondBin, thirdBin);
//                paridad = divider.XoR(paridad, fourthBin);
//                cout<<"Mensaje recuperado: ";
//                convertido += divider.BinToS(paridad);
//                convertido.pop_back();
//                convertido += divider.readData(secondPath);
//                convertido.pop_back();
//                convertido += divider.readData(thirdPath);
//                convertido.pop_back();
//                cout<<convertido<<endl;
//                break;
//            case 1:
//                paridad = divider.XoR(firstBin, thirdBin);
//                paridad = divider.XoR(paridad, fourthBin);
//                cout<<"Mensaje recuperado: ";
//                convertido += divider.readData(firstPath);
//                convertido.pop_back();
//                convertido += divider.BinToS(paridad);
//                convertido.pop_back();
//                convertido += divider.readData(thirdPath);
//                convertido.pop_back();
//                cout<<convertido<<endl;
//                break;
//            case 2:
//                paridad = divider.XoR(firstBin, secondBin);
//                paridad = divider.XoR(paridad, fourthBin);
//                cout<<"Mensaje recuperado: ";
//                convertido += divider.readData(firstPath);
//                convertido.pop_back();
//                convertido += divider.readData(secondPath);
//                convertido.pop_back();
//                convertido += divider.BinToS(paridad);
//                convertido.pop_back();
//                cout<<convertido<<endl;
//                break;
//            default:
//                break;
//        }
//    }



//    string size_ = to_string(file_size(name+to_string(0)+".dat"));
//    List<string> list = buildHuffmanTree("0"+size_);
//    json js = parseJson(list, "save");
//    cout<<js.dump()<<endl;
//    Client *client = Client::getInstance(globalPort);
//
//    string messageR = client->sendJson(js.dump());
//    json jmessageR = json::parse(messageR);
//
//    string message = jmessageR.value("message", "oops");
//    string pre = jmessageR.value("pre", "oops");
//    string in = jmessageR.value("in", "oops");
//
//    cout<<"message: "<<message<<endl;
//    cout<<"pre: "<<pre<<endl;
//    cout<<"in: "<<in<<endl;
//
//    List<string> pre_list = StoL(pre, '$');
//    List<string> in_list = StoL(in, '$');
//    preIndex = 0;
//    Tree_Node *root_ = buildTree(in_list, pre_list, 0, in_list.getSize()-1);
//
//    int index = -1;
//    string strDecode;
//    while (index < (int)message.size() - 2) {
//        decode(root_, index, message, &strDecode);
//    }
//    cout<<"message: "<<strDecode<<endl;
//    List<string> create = StoL(strDecode, '#');
//    divider.createDat(create.find(0)->getValue(), name, create.find(1)->getValue());
//
//    nameMD.insertLast(name+"$");
//    string metadata = LtoS(nameMD);
//
//    std::fstream toWrite;
//    toWrite.open("/home/ingrid/Documents/TECFileSystem/ControllerNode/METADATA.txt", std::ios::out | std::ios::binary);
//    toWrite.write(metadata.c_str(), metadata.size());
//    //Ahora cierro el archivo
//    toWrite.close();




    //meter la lista en un txt para la metadata

//    string convertido = "";
//    cout<<"Resultado "<<endl;
//    convertido += divider.readData("Dat1.dat");
//    convertido.pop_back();
//    convertido += divider.readData("Dat2.dat");
//    convertido.pop_back();
//    convertido += divider.readData("Dat3.dat");
//    convertido.pop_back();
//    cout<<convertido<<endl;

//    divider.strToBin(divider.result.at(0));
//    string first = divider.tot;
//    divider.strToBin(divider.result.at(1));
//    string second = divider.tot;
//    divider.strToBin(divider.result.at(2));
//    string third = divider.tot;
//
//    string paridad = divider.XoR(first, second);
//    paridad = divider.XoR(paridad, third);
//    cout<<"Bit de paridad: " <<paridad<<endl;


//    for (int i=0; i< list.getSize(); i++){
//        cout<<list.find(i)->getValue()<<endl;
//    }

    return 0;
}
