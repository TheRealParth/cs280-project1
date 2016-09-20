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
#include <regex>
using namespace std;


string line;
string output;
string rawInput = "";

regex newLineToken ("^\\.ll N");
regex dotLineToken ("^\\..+");
regex findWords ("\\S+");
smatch wordResults;


int isNewParagraph = 0;
int lineNum = 0;
int wordCount = 0;
int spaceCount = 0;
int lineLength = 60;


string getWords(string in){
    int k = 0;
    string currentWord =  "";
    regex_search(in, wordResults, findWords);
//    for(int i = 0; i < in.length(); i++){
////        if((in[i] == "<") && (i < in.length() - 2)){
////            if((in[i+1] == "p") && (in[i+2]==">")){
////                
////            }
////        }
//        if(in[i] == " "){
//            currentWord = "";
//            spaceCount++;
//            k++;
//            continue;
//        }
//    }
    for(int i=0; i< wordResults.length(); i++){
        cout << wordResults[i];
    }
    return in;
}

//string finalOutput(string in){
//    
//}

int main (int argc, const char * argv[]) {
    //Check if user has input file name...
    if(argc < 2){
        cout << "Missing filename.\n\n";
        return 0;
    }
    //fail if lineLength is not valid (must be no less than 10)
    if(lineLength < 10){
        cout << "Line length is too small.\n\n";
        return 0;
    }
    
    cout << "\nReading file: " << argv[1];
    cout << "\n\n";
    
    //Start reading the file...
    ifstream file (argv[1]);
    
    //Print original data and unformat most of the input.
    if (file.is_open()) {
        while ( getline (file,line) ) {
            if (regex_match(line,newLineToken) && (isNewParagraph == 0)) {
                rawInput += "<p>";
                isNewParagraph = 1;
                continue;
            } else if(regex_match(line,newLineToken) && (isNewParagraph == 1)) {
                continue;
            } else if (regex_match(line,dotLineToken))  {
                continue;
            } else {
                rawInput += line + ' ';
                isNewParagraph = 0;
            }
        }
        file.close();
    } else cout << "Unable to open file";
    getWords(rawInput);
    
    //Function to separate words into an array and count spaces.
    
    
    return 0;
}
