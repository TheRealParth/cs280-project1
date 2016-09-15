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
#include <string>
using namespace std;


string line;
string output;
string rawInput = "";
int lineCount = 0;


string removeSpace(string in) {
    int isSpace = 0;
    string out = "";
    for(int i = 0; i < in.length(); i++){
        if(in[i] == ' '){
            if (isSpace == 1) continue;
            else {
                out += in[i];
                isSpace = 1;
            }
        } else {
            out += in[i];
            isSpace = 0;
        }
    }
    return out;
}

int main (int argc, const char * argv[]) {
    cout << "Reading file: " << argv[1];
    cout << '\n';
    
    
    int lineSize = 40;
    ifstream file (argv[1]);
    
    size_t pos = 0;
    
    string s;
    
    if (file.is_open())
    {
        
        while ( getline (file,line) )
        {
            rawInput += line + ' ';
            
            
//            cout << line.length() << ' ';
            
            lineCount++;
        }
        string test = removeSpace("stuff     things haha");
        
        cout << test;
        file.close();
    }
    else cout << "Unable to open file";
    
    return 0;
}
