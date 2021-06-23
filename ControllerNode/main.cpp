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
List<string> StoL (string text){
    string insert;
    List<string> result;
    for (int i = 0; i < text.size(); i++) {
        if (text[i]=='/'){
            result.insertLast(insert);
            insert ="";
        }
        else{
            insert.push_back(text[i]);
        }
    }
    result.insertLast(insert);
    return result;
}
int main() {
    //ifstream input("/home/ingrid/Documents/TECFileSystem/ControllerNode/hola.txt", ios::binary);
    string input_ ="/home/usuario/Proyectos/TECFileSystem/ControllerNode/hola.txt";

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
    List<string> result = StoL(input_);
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

    return 0;
}
