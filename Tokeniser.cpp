//
// Created by gupta on 13-02-2020.
//

#include "Tokeniser.h"

Tokeniser::Tokeniser(string fileName):input(fileName){}
string delimiters = " \n\t";

bool Tokeniser::tokenComplete(char c, string &token){
    if(delimiters.find(c)!=string::npos) {
        return true;
    }
    token += c;
    return false;
}

string Tokeniser::getToken(){
    string temp = prevToken;
    skip();
    return temp;
}

void Tokeniser::skip(){
    string token = "";
    if(input.is_open()) {
        char c;
        while (input.get(c)) {
            if(tokenComplete(c, token) && token.size() > 0) {
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
}

bool Tokeniser::eof(){
    return input.eof();
}

