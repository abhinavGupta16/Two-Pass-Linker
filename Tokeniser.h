//
// Created by gupta on 13-02-2020.
//

#ifndef ASSIGNMENT1_TOKENISER_H
#define ASSIGNMENT1_TOKENISER_H

#include <bits/stdc++.h>

using namespace std;

class Tokeniser {
private:
    ifstream input;
    bool resetValues;
    int prevOffset;
    int prevLineNum;
    string prevToken;
public:
    bool tokenNotExpected;
    int tokenLength;
    int tokenLineNum;
    int tokenOffSet;
    int offset;
    int linenum;
    Tokeniser(string fileName);
    string getToken();
    bool tokenComplete(char c, string &token);
    void reset();
    bool eof();
    void skip();
    int getOffset();

};


#endif //ASSIGNMENT1_TOKENISER_H
