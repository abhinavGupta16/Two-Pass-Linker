//
// Created by gupta on 13-02-2020.
//

#include "Tokeniser.h"

Tokeniser::Tokeniser(string fileName):input(fileName){
    linenum = 1;
    offset = 0;
    prevOffset = 0;
    resetValues = false;
    tokenExpected = false;
    skip();
}
string delimiters = " \n\t";

bool Tokeniser::tokenComplete(char c, string &token){
    if(c == '\n'){
        resetValues = true;
    }
    if(delimiters.find(c)!=string::npos) {
        return true;
    }
    token += c;
    return false;
}

string Tokeniser::getToken(){
    string temp = prevToken;
    tokenLineNum = prevLineNum;
    tokenOffSet = prevOffset;
    skip();
    if(temp==""){
        tokenExpected = true;
    }
    return temp;
}

void Tokeniser::skip(){
    string token = "";
    if(resetValues){
        offset=0;
        linenum++;
        resetValues=false;
    }
    int tempOffSet = 1;
    if(input.is_open()) {
        char c;
        while (input.get(c)) {
            offset++;
            if(tokenComplete(c, token) && token.size() > 0) {
                tokenLength = token.size();
                prevLineNum = linenum;
                prevOffset = offset-1;
                prevToken = token;
                return;
            }
        }
    }
    prevToken = token;
}

void Tokeniser::reset(){
    input.clear();
    input.seekg(0, ios::beg);
    skip();
}

bool Tokeniser::eof(){
    return input.eof();
}

int Tokeniser::getOffset(){
    return input.tellg();
}
