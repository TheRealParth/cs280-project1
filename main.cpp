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
#include <queue>

using namespace std;

string line;
string output;
string rawInput = "";

regex justNumbers ("\\d\\d\\d?"); //finds 2 and 3 digit number


int lineNum = 0;
int wordCount = 0;
int maxLineLength = 60;


//------------------------------ SET PARAGRAPH/LINE ---------------------------------
int setPl(string in){
    regex linesizeMarker ("\\<[0-9]+[0-9]+[0-9]\?\\>"); //finds the custom <linesize> markers
    smatch smm;
    if(in.find("<p>") <= 120){
        return 1;
    } else if(regex_match(in, linesizeMarker)){
        if (regex_search(in, smm, justNumbers)) {
            string tempNum = "";
            int newMaxLine;
            for (auto x:smm) tempNum += x;
            newMaxLine = stoi(tempNum);
            if(newMaxLine > 9 && newMaxLine <= 120) maxLineLength = newMaxLine;
            return 2;
        }
    }
        return 0;
}
// ------------------------------ SPLIT WORDS ---------------------------------
string splitWord(string in, int lineLength){
    lineLength = lineLength - 1;
    int r = in.length() % lineLength;
    int lineCount = in.length()/lineLength;

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
    //print the split array
    string sWord = "";
    for(int j=0; j< lineCount; j++){
      sWord += newWord[j];
        if(j != lineCount) sWord+= "\n";
    }
    return sWord;
}
// ------------------------------ GET WORDS ---------------------------------
string *getWords(string in){
    smatch sm;
    regex wordReg ("\\s+"); //finds spaces, which we'll use to split up the words
    sregex_token_iterator reg_end;

    int i = 0;
    int tokenCount = 0;
    string tempy = "";
    for(sregex_token_iterator it(in.begin(), in.end(), wordReg, -1); it != reg_end; ++it) {
        tempy = it->str();
        if(tempy.find("<") < 120) tokenCount++;
        i++;
    }
    string *newWords = new string[i];
    wordCount = i;
    
    int j = 0;
    for(sregex_token_iterator it(in.begin(), in.end(), wordReg, -1); it != reg_end; ++it) {
        newWords[j] = it->str();
        j++;
    }
    return newWords;
}
// ------------------------------ MAIN ---------------------------------
int main (int argc, const char * argv[]) {
    queue<string> q;
    queue<string> temp;
    int isNewParagraph = 0;
    regex dotLineToken ("^\\..+"); //finds any line that starts with a .
    regex numberToken ("^\\.ll\\s((\\d\\d$)|([0-1][0-1][0-9]$|120$))"); //finds .ll N where N is > 9 but <= 120

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
                        rawInput += "> ";
                        break;
                    }
                }
                continue;
            } else if(regex_match(line,dotLineToken) && (isNewParagraph == 1)) {
                if(regex_match(line,numberToken)){
                    while (regex_search(line,m,justNumbers)) {
                        rawInput += "<";
                        for (auto x:m) rawInput += x;
                        rawInput += "> ";
                        break;
                    }
                }
                continue;
            } else if (regex_match(line, dotLineToken) && (isNewParagraph == 1))  {
                continue;
            } else if((line.length() == 0) && (isNewParagraph == 0)){
                rawInput += "<p> ";
                isNewParagraph = 1;
                continue;
            } else if((line.length() == 0) && (isNewParagraph == 1)){
                continue;
            } else {
                rawInput += line + ' ';
                isNewParagraph = 0;
            }
        }
        file.close();
    } else cout << "Unable to open file";
    
    string *words = getWords(rawInput);
    
    for(int i = 0; i < wordCount; i ++){
        q.push(words[i]);
    }
    int lineSize = 0;
    int gaps = 0;
    int spaces = 0;
    int rSpaces = 0;
    int lastLine = 0;
    string gap = " ";
    while(!q.empty()){
        lineSize=0;
        spaces = 0;
        gaps = 0;
        rSpaces = 0;
        gap = " ";
        if(setPl(q.front()) == 0){
            if(q.front().length() > maxLineLength){
                cout << splitWord(q.front(),maxLineLength);
                q.pop();
            } else {
                while(!q.empty()){
                    if(setPl(q.front()) == 0){
                        lineSize += q.front().length();
                        temp.push(q.front());
                        q.pop();
                        if((lineSize + q.front().length()) >= maxLineLength) {
                            temp.push("\n");
                            break;
                        }
                        lineSize+= 1;
                        lastLine = 0;
                        
                    } else {
                        temp.push("\n\n");
                        lastLine = 1;
                        q.pop();
                        break;
                    }
                }
                //print the temp stck here
                gaps = temp.size() - 1;
                spaces = (maxLineLength - lineSize) / gaps;
                rSpaces = maxLineLength - lineSize;
                string ts = "";
                string ts1 = "";
                string ts2 = "";
                if(spaces >= 1  && lastLine == 0){
                    if(q.front() == "<p>" || (q.front().find("<") < 120)) continue;
                    ts = q.front();
                    int j = 0;
                    while ( j < (rSpaces % gaps) + ((spaces-1) * gaps) - 2){
                        ts1 += ts[j];
                        lineSize++;
                        j++;
                    }
                    while ( j < q.front().length()){
                        ts2 += ts[j];
                        j++;
                    }
                    q.front() = ts2;
                    if(lineSize < maxLineLength && ts1.length() > 0)
                    ts1 += "-";
                    if(ts1.length() > 1){
                    string temp1 = temp.back();
                    temp.back() = ts1;
                    temp.push(temp1);
                    }
//                    temp.push(splitWord(q.front(), (rSpaces % gaps > 0)));
                    gaps = temp.size() + 1;
                    spaces = (maxLineLength - lineSize) / gaps;
                    rSpaces = maxLineLength - lineSize;
                }
                if(lastLine ==  0){
                    for(int i = 0; i < spaces; i ++){
                        gap += " ";
                    }
                }
                while(!temp.empty()){
                    if(temp.front() == "\n" || temp.front() == "\n\n") {cout << temp.front(); temp.pop(); continue;}
                    cout << temp.front();
                    if(rSpaces > 0 && lastLine == 0) {cout << " "; rSpaces--;}
                    if(temp.size() > 2) {
                        cout << gap;
                    }
                    temp.pop();
                }
                
            }
        } else {
            string s = q.front();
            q.pop();
            if(s=="<p>"){
                cout << endl;
            }
            
        }
    }

    return 0;
}
