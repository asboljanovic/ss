#include "../inc/assembler.h"

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
using namespace std;

// int main()
// {

//     Assembler assembler("in.txt" , "out.txt");
//     assembler.first_pass();
//     assembler.second_pass();

//     return 0;

// }

int main(int argc, char *argv[])
{

    if (argc != 4)
    {
        cout << "Greska! Ocekivana 2 operanda- izlaz, ulaz" << endl;
        return -1;
    }

    string first_par = argv[1];

    if ((first_par != "-o") && (first_par != "-O"))
    {
        cout << "Greska! Prvi fajl mora da je izlazni! " << endl;
        return -1;
    }

    string input = argv[3];
    string output = argv[2];

    Assembler assembler(input, output);
    assembler.first_pass();
    assembler.second_pass();

    return 0;
}
