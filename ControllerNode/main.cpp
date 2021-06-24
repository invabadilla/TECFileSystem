#include <iostream>
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

#include <thread>
#include <chrono>

using std::cout;
using std::endl;
using std::copy;
using std::this_thread::sleep_for;
using namespace std::chrono_literals;


using namespace tinyxml2;
using namespace filesystem;
using json = nlohmann::json;

int globalPort;

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
int main() {
    //string input_ ="/home/usuario/Proyectos/TECFileSystem/ControllerNode/hola.txt";
    string input_ = "/home/ingrid/Documents/TECFileSystem/ControllerNode/hola.txt";

    ifstream input(input_, ios::binary);
    vector<char> bytes(
            (istreambuf_iterator<char>(input)),
            (istreambuf_iterator<char>()));
    input.close();

    string texto;
    for (int i = 0; i < bytes.size(); i++) {
        texto+=bytes.at(i);
    }

    divider divider;

    // Print 3 equal parts of the string
    divider.divideString(const_cast<char *>(texto.c_str()), 3, texto);
    List<string> result = StoL(input_, '/');
    string name = result.find(result.getSize()-1)->getValue();
    name.erase(name.end()-4, name.end());
    divider.createDatcopy("", name);

    XMLDocument xml_doc;
    path temp = current_path().parent_path().parent_path();
    string dn =temp.string()+"/RAID/Parameters_TECFS_Disk.xml"; const char *document_name = dn.c_str();
    XMLError eResult = xml_doc.LoadFile(document_name);
    XMLNode* root = xml_doc.FirstChildElement("Parameters");
    XMLElement *port = root->FirstChildElement("port");
    globalPort = stoi(port->GetText());
    /**
    string size_ = to_string(file_size(name+to_string(0)+".dat"));
    List<string> list = buildHuffmanTree("0"+size_);
    json js = parseJson(list, "save");
    cout<<js.dump()<<endl;
    Client *client = Client::getInstance(globalPort);
    client->sendJson(js.dump());**/

    //**********************************************************************//

    remove("/home/ingrid/Documents/TECFileSystem/ControllerNode/cmake-build-debug/hola2.dat");


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
        // /home/ingrid/Documents/TECFileSystem/RAID/DiskNodes/DiskNode+i+/Block+j/name+i.dat
        string pathToRead = "/home/ingrid/Documents/TECFileSystem/ControllerNode/cmake-build-debug/hola" + to_string(i) + ".dat" ;
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
    fourthPath = "/home/ingrid/Documents/TECFileSystem/ControllerNode/cmake-build-debug/hola3.dat";
    fourthBin = convert("/home/ingrid/Documents/TECFileSystem/ControllerNode/cmake-build-debug/hola3.dat");


    string convertido = "";
    if(numError == -1){
        cout<<"Resultado: ";
        convertido += divider.readData(firstPath);
        convertido.pop_back();
        convertido += divider.readData(secondPath);
        convertido.pop_back();
        convertido += divider.readData(thirdPath);
        convertido.pop_back();
        cout<<convertido<<endl;
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
                cout<<convertido<<endl;
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
                cout<<convertido<<endl;
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
                cout<<convertido<<endl;
                break;
            default:
                break;
        }
    }






    string messageR = client->sendJson(js.dump());
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
