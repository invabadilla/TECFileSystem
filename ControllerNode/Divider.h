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
        strToBin(result.at(0));
        string first = tot;
        strToBin(result.at(1));
        string second = tot;
        strToBin(result.at(2));
        string third = tot;

        string paridad = XoR(first, second);
        paridad = XoR(paridad, third);
        result.push_back(paridad);

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
            if(bin.size() < 7 && bin.size()!=0){
                int ajuste = 7-bin.size();
                string strajuste = "";
                for (int j = 0; j < ajuste; ++j) {
                    strajuste += "0";
                }
                bin += strajuste;
            }

            reverse(bin.begin(), bin.end());
            tot += bin + " ";
            //cout << bin << " ";
        }
        //cout<<endl;

    }

    void createDatcopy(string path, string name){
        std::fstream toWrite;
        for (int i=0; i < 3;i++){
            toWrite.open(path+name+to_string(i)+".dat", std::ios::out | std::ios::binary);
            strToBin(result.at(i));
            toWrite.write(tot.c_str(), tot.size());
            //Ahora cierro el archivo
            toWrite.close();
        }
        toWrite.open(path+name+to_string(3)+".dat", std::ios::out | std::ios::binary);
        toWrite.write(result.at(3).c_str(), result.at(3).size());
        //Ahora cierro el archivo
        toWrite.close();
    }
    void createDat(string path, string name, string j){
        std::fstream toWrite;
        for (int i=0; i < 3;i++){
            toWrite.open(path+to_string(i)+"/Block"+j+"/"+name+to_string(i)+".dat", std::ios::out | std::ios::binary);
            strToBin(result.at(i));
            toWrite.write(tot.c_str(), tot.size());
            //Ahora cierro el archivo
            toWrite.close();
        }
        toWrite.open(path+to_string(3)+"/Block"+j+"/"+name+to_string(3)+".dat", std::ios::out | std::ios::binary);
        toWrite.write(result.at(3).c_str(), result.at(3).size());
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

    string BinToS(string bin){
        stringstream sstream(bin);
        string output;
        while(sstream.good())
        {
            bitset<7> bits;
            sstream >> bits;
            char c = char(bits.to_ulong());
            output += c;
        }
        return output;
    }


    string XoR(string first, string second){
        string final = "";

        if (first.size() == second.size()) {
            for (int i = 0; i < first.size(); ++i) {
                char first_bit = first[i];
                char second_bit = second[i];
                if(first_bit == '1' && second_bit == '1'){final += "0";}
                else if(first_bit == '1' && second_bit == '0'){final += "1";}
                else if(first_bit == '0' && second_bit == '0'){final += "0";}
                else if(first_bit == '0' && second_bit == '1'){final += "1";}
                else if(first_bit == ' ' && second_bit == ' '){final += " ";}
            }
        } else{

        }
        return final;
    }
};

#endif //TECFILESYSTEM_DIVIDER_H
