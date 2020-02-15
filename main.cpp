#include<iostream>
#include<string>
#include <bits/stdc++.h>
#include <map>
#include "Helper.h"
#include "Tokeniser.h"

using namespace std;

unordered_map<string, string> symbolMap;
vector<string> symbolMapOrder;
vector <pair<int, string>> memoryVec;
vector <string> warnings;
unordered_map<string, int> definedNotUsed;
vector<string> definedNotUsedOrder;
vector <pair<string, bool>> declarationVec;
unordered_map<string, bool> allDeclarationVec;

void readSym(Tokeniser *tokeniser, int globalOffset){
    string sym = checkstring(tokeniser->getToken());
    int value = convertToNum(tokeniser->getToken());
    try{
        if(sym.size()>16){
            throw 4;
        }
        if(value == 100000000){
            throw 2;
        }
    } catch (int e){
        cout<<parseError(e,1,1);
        exit(e);
    }

    if(symbolMap.find(sym) == symbolMap.end()) {
        symbolMapOrder.push_back(sym);
        symbolMap[sym] = to_string(globalOffset + value);
    } else {
        symbolMap[sym] = symbolMap[sym] + " " + errorMessages(2);
    }
}

void readDecal(Tokeniser *tokeniser){
    string symbol = checkstring(tokeniser->getToken());
    if(symbol.size()>16){
        throw 4;
    }
    declarationVec.push_back(make_pair(symbol, false));
}

void pass1(Tokeniser *tokeniser){
    int globalOffset = 0;
    while(!tokeniser->eof()) {
        try {
            int defcount = checkDefCount(tokeniser->getToken());
            for (int j = 0; j < defcount; j++) {
                readSym(tokeniser, globalOffset);
            }

            int usecount = checkUseCount(tokeniser->getToken());
            for (int j = 0; j < usecount; j++) {
                readDecal(tokeniser);
            }

            int instcount = checkInstCount(tokeniser->getToken());
            for (int j = 0; j < instcount; j++) {
                globalOffset++;
                string addressMode = checkAddress(tokeniser->getToken());
                int value = convertToNum(tokeniser->getToken());
            }
        }
        catch (int e) {
            int lineoffset = tokeniser->tokenOffSet-tokeniser->tokenLength+1;
            if(tokeniser->tokenExpected){
                lineoffset = tokeniser->tokenOffSet+1;
            }
            cout << parseError(e, tokeniser->tokenLineNum, lineoffset);
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
            readDecal(tokeniser);
        }

        int instcount = stoi(tokeniser->getToken());
        for (int j = 0; j < instcount; j++) {
            string addressMode = tokeniser->getToken();
            int value = stoi(tokeniser->getToken());
            int operand = value % 1000;
            int opcode = value / 1000;
            string errorMsg = "";
            bool flag = false;
            int errorCode = -1;
            if (opcode >= 10) {
                errorCode = 11;
                value = 9999;
                operand = value % 1000;
                opcode = value / 1000;
                errorMsg = errorMessages(errorCode);
                flag = true;
            }
            if (!addressMode.compare("R")) {
                if (operand >= instcount && !flag) {
                    value = opcode * 1000;
                    errorMsg = errorMessages(9);
                }
                memoryVec.push_back(make_pair(value + moduleBase, errorMsg));
            } else if (!addressMode.compare("E")) {
                processOperandE(operand, opcode, symbolMap, memoryVec, declarationVec);
            } else if (!addressMode.compare("I")) {
                processImmediate(memoryVec, value, errorCode);
            } else if (!addressMode.compare("A")) {
                if (operand >= 512 && !flag) {
                    value = opcode * 1000;
                    errorMsg = errorMessages(8);
                }
                memoryVec.push_back(make_pair(value, errorMsg));;
            }
            globalOffset++;
        }
        if (instcount > 0) {
            moduleBase = globalOffset;
            checkDeclarationVec(declarationVec, memoryVec, moduleNo, allDeclarationVec);
            moduleNo++;
        }
    }
    addDefinedNotUsedWarning(definedNotUsed, warnings, definedNotUsedOrder, allDeclarationVec);
}


int main(){
    string fileName = "D:\\NYU_assignment\\Spring_2020\\OS\\lab1samples\\temp";

    Tokeniser tokeniser(fileName);

    pass1(&tokeniser);

    printSymbolTable(symbolMap, symbolMapOrder);
    cout<<endl;

    // PASS 2
    tokeniser.reset();
    pass2(&tokeniser);

    printMemoryVector(memoryVec);
    cout<<endl;
    printVector(warnings);
    cout<<endl<<endl;
    cout<<"end";
    return 0;
}
