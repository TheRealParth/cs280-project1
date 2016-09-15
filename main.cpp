//
//  main.cpp
//  CS280 proj 1
//
//  Created by parth patel on 9/11/16.
//
//

#include "main.hpp"
#include <iostream>
#include <fstream>
using namespace std;
int main (int argc, const char * argv[]) {
    cout << argv[1];
    cout << '\n';
    char * memblock;
    
    ifstream file (argv[1], ios::in|ios::binary|ios::ate);
    if (file.is_open())
    {
        int size = file.tellg();
        memblock = new char[size];
        file.seekg (0, ios::beg);
        file.read (memblock, size);
        file.close();
        
        cout << memblock;
        
        delete[] memblock;
    }
    else cout << "Unable to open file";
    return 0;
}