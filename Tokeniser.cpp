//
// Created by gupta on 13-02-2020.
//

#include "Tokeniser.h"

Tokeniser::Tokeniser(string fileName):input(fileName){
    linenum = 1;
    offset = 0;
    prevOffset = 0;
    resetValues = false;
    tokenNotExpected = false;
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
    tokenLength = temp.size();
    skip();
    if(temp==""){
        tokenNotExpected = true;
    }
    return temp;
}

void Tokeniser::skip(){
    string token = "";
    if(input.is_open()) {
        char c;
        while (input.get(c)) {
            if(resetValues){
                offset=0;
                linenum++;
                resetValues=false;
            }
            offset++;
            if(tokenComplete(c, token) && token.size() > 0) {
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
