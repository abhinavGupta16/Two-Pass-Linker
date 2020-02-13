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
    string prevToken;
public:
    Tokeniser(string fileName);
    string getToken();
    bool tokenComplete(char c, string &token);
    void reset();
    bool eof();
    void skip();

};


#endif //ASSIGNMENT1_TOKENISER_H
