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

int main() {

    //ifstream input("/home/usuario/Proyectos/TECFileSystem/ControllerNode/hola.txt", ios::binary);
    ifstream input("/home/ingrid/Documents/TECFileSystem/ControllerNode/hola.txt", ios::binary);
    vector<char> bytes(
            (istreambuf_iterator<char>(input)),
            (istreambuf_iterator<char>()));
    input.close();

    string texto;
    for (int i = 0; i < bytes.size(); ++i) {
        texto+=bytes.at(i);
    }
    cout<<texto<<endl;

    divider divider;



    // Print 3 equal parts of the string
    divider.divideString(const_cast<char *>(texto.c_str()), 3, texto);

    divider.createDat();

    string convertido = "";
    cout<<"Resultado "<<endl;
    convertido += divider.readData("Dat1.dat");
    convertido.pop_back();
    convertido += divider.readData("Dat2.dat");
    convertido.pop_back();
    convertido += divider.readData("Dat3.dat");
    convertido.pop_back();
    cout<<convertido<<endl;

    divider.strToBin(divider.result.at(0));
    string first = divider.tot;
    divider.strToBin(divider.result.at(1));
    string second = divider.tot;
    divider.strToBin(divider.result.at(2));
    string third = divider.tot;

    string paridad = divider.XoR(first, second);
    paridad = divider.XoR(paridad, third);
    cout<<"Bit de paridad: " <<paridad<<endl;


    XMLDocument xml_doc;
    path temp = current_path().parent_path().parent_path();
    string dn =temp.string()+"/RAID/Parameters_TECFS_Disk.xml"; const char *
            document_name = dn.c_str();
    XMLError eResult = xml_doc.LoadFile(document_name);
    XMLNode* root = xml_doc.FirstChildElement("Parameters");
    XMLElement *port = root->FirstChildElement("port");
    globalPort = stoi(port->GetText());

    List<string> toSend = buildHuffmanTree("hola");
    for (int i = 0; i < toSend.getSize(); ++i) {
        cout<<toSend.find(i)<<" ";
    }
    cout<<endl;
    json JsonToSend = parseJson(toSend, "key");

    Client *client = Client::getInstance(globalPort);
    client->sendJson(to_string(JsonToSend));


    return 0;
}
