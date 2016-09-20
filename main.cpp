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
string space = " ";
string rawInput = "";


regex newLineToken ("^\\.ll N");
regex dotLineToken ("^\\..+");
regex newParagraphToken ("<p>");


int isNewParagraph = 0;
int lineNum = 0;
int wordCount = 0;
int spaceCount = 0;


//USE LATER
//        if((in[i] == "<") && (i < in.length() - 2)){
//            if((in[i+1] == "p") && (in[i+2]==">")){
//                
//            }
//        }

string *splitWord(string in, int lineLength){
    int r = in.length() % lineLength;
    int divider = in.length()/lineLength;
    
    string *newWord;
    if(r>0) newWord = new string[divider + 1];
    else newWord = new string[divider];
    
    
    for(int i = 0; i < divider-1; i++ ){
        for(int j = lineLength*(i-1); j < lineLength; j++){
            newWord[i] += in[j];
            
        }
        if((i % lineLength) == 0) {
            newWord[i] += "-\n";
        }
        
    }
    
    cout << "\n Remainder: " << r;
    cout << "\n Divider: " << divider << "\n";
    
    return newWord;
}


//Breaks up the string into a string array of words
string *getWords(string in){
    
    int isWord = 1;
    
    //Count words and spaces;
    for(int i = 0; i < in.length(); i++){
        if(in[i] == ' '){
            if(isWord){
                isWord = 0;
                wordCount++;
            }
            spaceCount++;
            continue;
        }
        isWord = 1;
    }
    cout << "\nWord count: " << wordCount;
    cout << "\nSpace count: " << spaceCount << "\n";
    
    //Set the base space distance between words based on the amount of evenly divisible spaces
    int wordGap = spaceCount/wordCount;
    for(int i = 1; i < wordGap; i ++){
        space += " ";
    }
    
    //Create an array of words
    string *words = new string[wordCount];
    isWord = 1;
    int k = 0;
    words[k]="";
    for(int j = 0; j < in.length(); j++){
        if(in[j] == ' '){
            if(isWord == 1){
                isWord = 0;
                if(k< wordCount)
                k++;
                words[k]="";
            } else {
                continue;
            }
        } else {
            isWord = 1;
            words[k] += in[j];
        }
    }
    return words;
}

//string finalOutput(string in){
//    
//}

int main (int argc, const char * argv[]) {
    int lineLength = 20;
    
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
            if (regex_match(line,newLineToken) && (isNewParagraph == 0) && (lineNum > 0)) {
                rawInput += "<p>";
                isNewParagraph = 1;
                continue;
            } else if(regex_match(line,newLineToken) && (isNewParagraph == 1)) {
                continue;
            } else if (regex_match(line,dotLineToken))  {
                continue;
            }else {
                rawInput += line + ' ';
                isNewParagraph = 0;
            }
            lineNum++;
        }
        file.close();
    } else cout << "Unable to open file";
     string *words = getWords(rawInput);
    
    for(int i = 0; i < wordCount; i++){
        if(words[i].length() > lineLength)
        {
            string *split = splitWord(words[i], lineLength);
            for(int j=0; j<(sizeof(split[0])/sizeof(split)); j++){
                cout << split[j];
            }
            
        }
        else
        cout << words[i] + space;
    }
    
    //Function to separate words into an array and count spaces.
    
    
    return 0;
}
