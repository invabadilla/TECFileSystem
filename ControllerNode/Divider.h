//
// Created by ingrid on 6/17/21.
//

#ifndef TECFILESYSTEM_DIVIDER_H
#define TECFILESYSTEM_DIVIDER_H

#include <iostream>
#include <sstream>
#include<string>
#include <sys/stat.h>
#include <algorithm>
#include <cstring>
#include "vector"
#include "bitset"

using namespace std;
class divider
{

// Function to print n equal parts of str
public:
    vector<string> result;
    string tot = "";

    void divideString(char str[], int n, string texto)
    {
        string temp;

        int str_size = strlen(str);
        int i;
        int part_size;

        // Check if string can be divided in
        // n equal parts
        if (str_size % n != 0)
        {
            texto.push_back(' ');
            divideString(const_cast<char *>(texto.c_str()), 3, texto);
            return;
        }

        // Calculate the size of parts to
        // find the division points
        part_size = str_size / n;
        for (i = 0; i< str_size; i++)
        {
            if (i % part_size == 0){
                //cout << endl;
                if (temp != "") { result.push_back(temp); }
                temp = "";
            }
            //cout << str[i];
            temp += str[i];
        }
        //cout<<endl;
        result.push_back(temp);
    }

    void strToBin(string s)
    {
        int n = s.length();
        tot = "";
        for (int i = 0; i <= n; i++)
        {
            // convert each char to
            // ASCII value
            int val = int(s[i]);

            // Convert ASCII value to binary
            string bin = "";
            while (val > 0)
            {
                (val % 2)? bin.push_back('1') :
                bin.push_back('0');
                val /= 2;
            }


            reverse(bin.begin(), bin.end());
            tot += bin + " ";
            //cout << bin << " ";
        }
        //cout<<endl;

    }

    void createDat(){
        std::fstream toWrite;

        toWrite.open("Dat1.dat", std::ios::out | std::ios::binary);
        strToBin(result.at(0));
        toWrite.write(tot.c_str(), tot.size());
        //Ahora cierro el archivo
        toWrite.close();

        toWrite.open("Dat2.dat", std::ios::out | std::ios::binary);
        strToBin(result.at(1));
        toWrite.write(tot.c_str(), tot.size());
        //Ahora cierro el archivo
        toWrite.close();

        toWrite.open("Dat3.dat", std::ios::out | std::ios::binary);
        strToBin(result.at(2));
        toWrite.write(tot.c_str(), tot.size());
        //Ahora cierro el archivo
        toWrite.close();
    }

    string readData(string Path){
        std::fstream toRead;

        //Leemos el archivo
        toRead.open(Path, std::ios::in | std::ios::binary);
        toRead.seekg(0, toRead.end);
        int tam = toRead.tellg();
        toRead.seekg(0, toRead.beg);
        //Para almacenar los datos
        char * datos = new char[tam];

        //Leemos el archivo
        toRead.read(datos, tam);

        string cadena = {};
        cadena.assign(datos, tam );

        stringstream sstream(cadena);
        string output;
        while(sstream.good())
        {

            bitset<7> bits;
            sstream >> bits;
            char c = char(bits.to_ulong());
            output += c;
        }


        //Cerramos el archivo
        toRead.close();
        delete[] datos;
        return output;
    }
};

#endif //TECFILESYSTEM_DIVIDER_H
