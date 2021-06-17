#include <iostream>
#include "RAID_5.h"
#include "ControllerNode/Divider.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    //RAID_5 *raid = new RAID_5();

    ifstream input("/home/ingrid/Documents/TECFileSystem/mytext.txt", ios::binary);
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

//**************************************************************************************************//


    // Print 3 equal parts of the string
    divider.divideString(const_cast<char *>(texto.c_str()), 3, texto);

//**************************************************************************************************//
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

    return 0;
}
