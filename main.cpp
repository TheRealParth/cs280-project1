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
// ------------------------------ GET LINE WORDS ---------------------------------
//Recursively calculates how many words can fit into one line including 1 space per word.
int getLineWords(string *words, int start = 0, int lineSize = 0){
    if((start+1) < wordCount){
        int currSize = words[start].length() + 1 + lineSize;
        if(currSize > maxLineLength){
            return start;
        } else if(currSize == maxLineLength) {
            return start+1;
        } else if (words[start+1].find("<") <= 120){
            return start+1;
        } else {
            return getLineWords(words, start+1, currSize);
        }
    } else {
        return start;
    }
}


// ------------------------------ SPLIT WORDS ---------------------------------
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
    //print the split array
    for(int j=0; j< lineCount; j++){
      cout << newWord[j] << "\n";
    }
}

// ------------------------------ GET WORDS ---------------------------------
string *getWords(string in){
    smatch sm;
    regex wordReg ("\\s+"); //finds spaces, which we'll use to split up the words
    
    int pos = 0;
    
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
//------------------------------ GET LINE SIZE ---------------------------------
int getLineSize(string *in, int start, int stop){
    int size = 0;
    int i = start;
    while(i < stop){
        if(in[i].find("<") < 121){
            i++;
            continue;
        }
        size += in[i].length();
        i++;
    }
    return size;
}
//------------------------------ LINE BUILDER ---------------------------------
int lineBuilder(string *in, int start, int stop){
    int gaps = ((stop - start) - 1);
    int lineSize = 0;
    int size = 0;
    string gap = "";
    int i = start;
    int check = 0;
    int isNewParagraph = 0;
    
    lineSize = getLineSize(in, start, stop);
    

    int remainingSpace = maxLineLength - lineSize;
    float spaces = 0;
    if(gaps > 0)
    spaces = (float) remainingSpace / (float) gaps;

//        cout << "start: " << start << "  ";
//        cout << "stop: " << stop << "   |";
//    cout << "l: " << lineSize << " ";
//    cout << "s: " << spaces << " ";
//    cout << "g: " << gaps << " ";
//    cout << "r: " << remainingSpace << " |   \n";

    if(spaces > 3) {
        gap = " ";
        //SPLIT THE NEXT WORD
    } else if (spaces == 3){
        if((remainingSpace % gaps) == 0){
            gap = "   ";
        } else {
            gap = " ";
            //SPLIT THE NXT WORD
        }
    } else if (spaces < 3){
        for(int i =0; i < spaces; i++){
            gap += " ";
        }
    }
    check = 0;
    isNewParagraph = 0;
    i = start;
    while(i < stop) {
        if(in[i].length() > maxLineLength) {
            splitWord(in[i], maxLineLength);
            i++;
            continue;
        }
        check = setPl(in[i]);
        
        if((check > 0) && (isNewParagraph ==0)) {
            if(check == 1) cout << "\n\n";
            if(check == 2) cout << "\n";
            check = 0;
            isNewParagraph = 1;
        } else if ((check > 0) && (isNewParagraph ==1)){
        } else {
            isNewParagraph = 0;
            cout << in[i] << gap;
        }
        i++;
    }
    if(isNewParagraph == 0)
//    cout << endl;
    
    return stop;
}

// ------------------------------ MAIN ---------------------------------
int main (int argc, const char * argv[]) {
    queue<string> q;
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
                } else {
                    rawInput += "<p> ";
                    isNewParagraph = 1;
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
            } else if (regex_match(line, dotLineToken))  {
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
    
    
    int start = 0;
    int stop = 0;
    
    for(int i = 0; i < wordCount; i++){
        stop = getLineWords(words, i, 0);

        start = lineBuilder(words, start, stop);
        i = start;
    }

    return 0;
}
