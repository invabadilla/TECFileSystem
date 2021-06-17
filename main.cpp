#include <iostream>
#include <algorithm>
#include <iterator>
#include <numeric>
#include "functional"

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

    divider.strToBin(divider.result.at(0));
    string first = divider.tot;
    divider.strToBin(divider.result.at(1));
    string second = divider.tot;
    divider.strToBin(divider.result.at(2));
    string third = divider.tot;

    string paridad = divider.XoR(first, second);
    paridad = divider.XoR(paridad, third);
    cout<<"Bit de paridad: " <<paridad<<endl;

    return 0;
}
