#include<iostream>
#include<string>
#include <bits/stdc++.h>
#include "Helper.h"
#include "Tokeniser.h"

using namespace std;

// Stores the Symbol Map and value
unordered_map<string, string> symbolMap;
// Maintains the order in which the variables were added to the symbolMap
vector<string> symbolMapOrder;
// Vector to maintain the Symbol and declared value
vector<pair<string, int>> origSymbolValuePair;
// Vector maintains the Memory Map and error
vector <pair<int, string>> memoryVec;
// Vector to add warnings
vector <string> warnings;
// Track of variables which were defined but not used
unordered_map<string, int> definedNotUsed;
// Vector to maintain the order of definedNotUsed Map
vector<string> definedNotUsedOrder;
// Vector to keep track of variables declared in a particular module
vector <pair<string, bool>> declarationVec;
// Vector to keep track of variables declared so far
unordered_map<string, bool> allDeclarationVec;

/*
 * Reads the Definition List
 */
void readSym(Tokeniser *tokeniser, int globalOffset){
    string sym = checkstring(tokeniser->getToken());
    int value = convertToNum(tokeniser->getToken());

    if(sym.size()>16){
        throw 4;
    }

    if(symbolMap.find(sym) == symbolMap.end()) {
        symbolMapOrder.push_back(sym);
        origSymbolValuePair.push_back(make_pair(sym, value));
        symbolMap[sym] = to_string(globalOffset + value);
    } else {
        symbolMap[sym] = symbolMap[sym] + " " + errorMessages(2);
    }
}

// Reads the UseList
void readUseList(Tokeniser *tokeniser){
    string symbol = checkstring(tokeniser->getToken());
    if(symbol.size()>16){
        throw 4;
    }
    declarationVec.push_back(make_pair(symbol, false));
}

void pass1(Tokeniser *tokeniser){
    int globalOffset = 0;
    int moduleNo = 1;
    int totalInstCount = 0;
    while(!tokeniser->eof()) {
        try {
            origSymbolValuePair.clear();
            int defcount = checkDefCount(tokeniser->getToken());
            for (int j = 0; j < defcount; j++) {
                readSym(tokeniser, globalOffset);
            }

            int usecount = checkUseCount(tokeniser->getToken());
            for (int j = 0; j < usecount; j++) {
                readUseList(tokeniser);
            }

            int instcount = checkInstCount(tokeniser->getToken(), totalInstCount);
            checkForRule5(origSymbolValuePair, warnings, instcount, moduleNo, symbolMap);
            for (int j = 0; j < instcount; j++) {
                globalOffset++;
                string addressMode = checkAddress(tokeniser->getToken());
                int value = convertToNum(tokeniser->getToken());
            }
            moduleNo++;
        }
        catch (int e) {
            int lineoffset = tokeniser->tokenOffSet-tokeniser->tokenLength+1;
            int lineNum = tokeniser->tokenLineNum;
            if(tokeniser->tokenNotExpected){
                string s = tokeniser->getToken();
                lineoffset = tokeniser->offset;
                lineNum = tokeniser->linenum;
            }
            cout << parseError(e, lineNum, lineoffset);
            exit(3);
        }
    }
}

void pass2(Tokeniser *tokeniser){
    int globalOffset = 0;
    int moduleBase = 0;
    int moduleNo = 1;
    while(!tokeniser->eof()) {
        int defcount = convertToNum(tokeniser->getToken());
        for (int j = 0; j < defcount; j++) {
            string symbol = tokeniser->getToken();
            if(definedNotUsed.find(symbol) == definedNotUsed.end()){
                definedNotUsedOrder.push_back(symbol);
                definedNotUsed[symbol] = moduleNo;
            }
            tokeniser->skip();
        }

        int usecount = stoi(tokeniser->getToken());
        declarationVec.clear();
        for (int j = 0; j < usecount; j++) {
            readUseList(tokeniser);
        }

        int instcount = stoi(tokeniser->getToken());
        for (int j = 0; j < instcount; j++) {
            string addressMode = tokeniser->getToken();
            int value = stoi(tokeniser->getToken());
            int operand = value % 1000;
            int opcode = value / 1000;
            string errorMsg = "";
            int errorCode = -1;
            if (opcode >= 10 && addressMode.compare("I")) {
                errorCode = 11;
                value = 9999;
                operand = value % 1000;
                opcode = value / 1000;
                addressMode = "I";
            }
            if (!addressMode.compare("R")) {
                if (operand >= instcount) {
                    value = opcode * 1000;
                    errorMsg = errorMessages(9);
                }
                memoryVec.push_back(make_pair(value + moduleBase, errorMsg));
            } else if (!addressMode.compare("E")) {
                processOperandE(operand, opcode, symbolMap, memoryVec, declarationVec);
            } else if (!addressMode.compare("I")) {
                processImmediate(memoryVec, value, errorCode);
            } else if (!addressMode.compare("A")) {
                if (operand >= 512) {
                    value = opcode * 1000;
                    errorMsg = errorMessages(8);
                }
                memoryVec.push_back(make_pair(value, errorMsg));;
            }
            globalOffset++;
        }
        if (instcount > 0) {
            moduleBase = globalOffset;
        }
        checkDeclarationVec(declarationVec, memoryVec, moduleNo, allDeclarationVec);
        moduleNo++;
    }
    addDefinedNotUsedWarning(definedNotUsed, warnings, definedNotUsedOrder, allDeclarationVec);
}


int main(int argc, char *argv[]){

    if(argc!=2){
        cout<<"Expected argument after options"<<endl;
        return 0;
    }
    string fileName = argv[1];
    Tokeniser tokeniser(fileName);

    pass1(&tokeniser);
    printWarnings(warnings);
    printSymbolTable(symbolMap, symbolMapOrder);
    warnings.clear();
    cout<<endl;

    // PASS 2
    tokeniser.reset();
    pass2(&tokeniser);
    printMemoryVector(memoryVec);
    cout<<endl;
    printWarnings(warnings);
    cout<<endl<<endl;
    return 0;
}
