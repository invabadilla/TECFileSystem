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

/**
 * Class to split the file for storage in the blocks
 */
class divider
{

public:
    vector<string> result;
    string tot = "";
    int parity = 0;

    /**
     * Function to split n equal parts of str
     */
    void divideString(char str[], int n, string texto)
    {
        string temp;
        int str_size = strlen(str);
        int i;
        int part_size;

        // Check if string can be divided in n equal parts
        if (str_size % n != 0)
        {
            texto.push_back(' ');
            divideString(const_cast<char *>(texto.c_str()), 3, texto);
            return;
        }

        // Calculate the size of parts to find the division points
        part_size = str_size / n;
        for (i = 0; i< str_size; i++)
        {
            if (i % part_size == 0){
                if (temp != "") { result.push_back(temp); }
                temp = "";
            }
            temp += str[i];
        }
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

    /**
     * Parse the string data to binary data to store it in blocks
     * @param s
     */
    void strToBin(string s)
    {
        int n = s.length();
        tot = "";
        for (int i = 0; i <= n; i++)
        {
            // convert each char to ASCII value
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
        }
    }

    /**
     * Create the directory to save the binary information
     * @param path where it is stored
     * @param name from the file
     */
    void createDatcopy(string path, string name){
        std::fstream toWrite;
        for (int i=0; i < 3;i++){
            toWrite.open(path+name+to_string(i)+".dat", std::ios::out | std::ios::binary);
            strToBin(result.at(i));
            toWrite.write(tot.c_str(), tot.size());
            toWrite.close();
        }
        toWrite.open(path+name+to_string(3)+".dat", std::ios::out | std::ios::binary);
        toWrite.write(result.at(3).c_str(), result.at(3).size());
        toWrite.close();
    }

    /**
     * Create the directory to save the binary information in the RAID
     * @param path where it is stored
     * @param name from the file
     * @param j Number of the block in the RAID
     */
    void createDat(string path, string name, string j){
        std::fstream toWrite;
        for (int i=0; i < 3;i++){
            toWrite.open(path+to_string(i)+"/Block"+j+"/"+name+to_string(i)+".dat", std::ios::out | std::ios::binary);
            strToBin(result.at(i));
            toWrite.write(tot.c_str(), tot.size());
            toWrite.close();
        }
        toWrite.open(path+to_string(3)+"/Block"+j+"/"+name+to_string(3)+".dat", std::ios::out | std::ios::binary);
        toWrite.write(result.at(3).c_str(), result.at(3).size());
        toWrite.close();
    }

    /**
     * Read binary data in the file
     * @param Path of the file
     * @return information in the file
     */
    string readData(string Path){
        std::fstream toRead;

        toRead.open(Path, std::ios::in | std::ios::binary);
        toRead.seekg(0, toRead.end);
        int tam = toRead.tellg();
        toRead.seekg(0, toRead.beg);
        //Para almacenar los datos
        char * datos = new char[tam];

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

        toRead.close();
        delete[] datos;
        return output;
    }

    /**
     * Parse binary information to string data
     * @param bin binary string to parse
     * @return string with the information
     */
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

    /**
     * Parity calculation of the binary information
     * @param first binary information
     * @param second binary information
     * @return parity in the first and the second
     */
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

    /**
     * Search the block to store the parity information
     * @return number of the block
     */
    string DiskParity(){
        //block;
        return "hola";

    }
};

#endif //TECFILESYSTEM_DIVIDER_H
