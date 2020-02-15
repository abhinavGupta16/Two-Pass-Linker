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
public:
    bool tokenNotExpected;
    string prevToken;
    int prevOffset;
    int prevLineNum;
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
