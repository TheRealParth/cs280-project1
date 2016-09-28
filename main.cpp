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
#include <stdlib.h>     /* srand, rand */

using namespace std;


string line;
string output;
string space = " ";
string rawInput = "";

//regex newParagraphToken ("<p>");
regex wordReg ("\\S+\\s?");

int isNewParagraph = 0;
int lineNum = 0;
int wordCount = 0;
int spaceCount = 0;
int rSpaces = 0;

void splitWord(string in, int lineLength){
    lineLength = lineLength - 1;
    int r = in.length() % lineLength;
    int lineCount = in.length()/lineLength;
    
        //if there is 1 character left, it will be appended at the end of
        //the last line instead of the dash, if there are more add another line.
        if(r > 1){
            lineCount++;
        }
    
        string *newWord = new string[lineCount];
    
        int pos = 0;

        for(int i = 0; i < lineCount; i++){
            while(pos < lineLength + (lineLength * i)){
                //if the position get out of bounds end the loop.
                if(pos > in.length()){
                    break;
                }
                newWord[i] += in[pos];
                pos++;
            }
            //If there is only 1 letter left, append it to the last line.
            if((r == 1) && (i == lineCount - 1)){
                newWord[i] += in[pos];
            } else if(i != lineCount - 1) {
                newWord[i] += "-";
            }
        }
        if(r < 1){
            
        }
        //print the split array
        for(int j=0; j< lineCount; j++){
                            cout << newWord[j] << "\n";
        }
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
    
    //Set the base space distance between words based on the amount of evenly divisible spaces
    int wordGap = spaceCount/wordCount;
    //set remaining spaces to insert randomly later
    rSpaces = spaceCount % wordCount;
    
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
    int maxLineLength = 20;
    
    regex lineLengthToken ("^\\.ll+");
    regex dotLineToken ("^\\..+");
    regex numberToken ("^\\.ll\\s((\\d\\d$)|([0-1][0-1][0-9]$|120$))");
    regex justNumbers ("\\d\\d\\d?");
    smatch m;
    //Check if user has input file name...
    if(argc < 2){
        cout << "Missing filename.\n\n";
        return 0;
    }
    //fail if maxLineLength is not valid (must be no less than 10)
    if((maxLineLength < 10) || (maxLineLength > 120)){
        cout << "Line length is invalid.\n\n";
        return 0;
    }

    cout << "\nReading file: " << argv[1];
    cout << "\n\n";
    
    //Start reading the file...
    ifstream file (argv[1]);
    
    //Print original data and unformat most of the input.
    if (file.is_open()) {
        while ( getline (file,line) ) {
            if (regex_match(line,dotLineToken) && (isNewParagraph == 0)) {
                if(regex_match(line,numberToken)){
                    while (regex_search(line,m,justNumbers)) {
                        rawInput += "<";
                        for (auto x:m) rawInput += x;
                        rawInput += ">";
                        break;
                    }
                } else {
                rawInput += "<p>";
                isNewParagraph = 1;
                }
                continue;
            } else if(regex_match(line,dotLineToken) && (isNewParagraph == 1)) {
                if(regex_match(line,numberToken)){
                    while (regex_search(line,m,justNumbers)) {
                        rawInput += "<";
                        for (auto x:m) rawInput += x;
                        rawInput += ">";
                        break;
                    }
                }
                continue;
            } else if (regex_match(line,dotLineToken))  {
                continue;
            } else if((line.length() == 0) && (isNewParagraph == 0)){
                rawInput += "<p>";
                isNewParagraph = 1;
                continue;
            }else if((line.length() == 0) && (isNewParagraph == 1)){
                continue;
            }else {
                rawInput += line + ' ';
                isNewParagraph = 0;
            }
        }
        file.close();
    } else cout << "Unable to open file";
     string *words = getWords(rawInput);
    
    for(int i = 0; i < wordCount; i++){
        if(words[i].length() > maxLineLength)
        {
            splitWord(words[i], maxLineLength);
            
        }
        else
        cout << words[i] + space;
    }
    
    cout << "\n\nRemaining Spaces: " << rSpaces << "\n";
    //Function to separate words into an array and count spaces.
    
    
    return 0;
}
