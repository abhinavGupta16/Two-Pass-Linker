#include<iostream>
#include<string>
#include <bits/stdc++.h>
#include <map>
#include "Helper.h"
#include "Tokeniser.h"

using namespace std;

map<string, string> symbolMap;
vector <pair<int, string>> memoryVec;
vector <string> warnings;
map<string, bool> definedNotUsed;
vector <pair<string, bool>> declarationVec;

void readSym(Tokeniser *tokeniser, int globalOffset){
    int value;
    string sym = tokeniser->getToken();
    string val = tokeniser->getToken();
    try{
        if(sym.size()>16){
            throw 4;
        }
        if(val == "tokens.size()"){
            throw 2;
        }
        value = stoi(val);
    } catch (int e){
        cout<<parseError(e,1,1);
        exit(e);
    }

    if(symbolMap.find(sym) == symbolMap.end()) {//|| !symbolMap[tokens[traverseVec]].compare(notDeclared)){
        symbolMap[sym] = to_string(globalOffset + value);
    } else {
        symbolMap[sym] = symbolMap[sym] + " " + errorMessages(2);
    }
}

void readDecal(Tokeniser *tokeniser){
      declarationVec.push_back(make_pair(tokeniser->getToken(), false));
//    if(symbolMap.find(tokens[traverseVec]) == symbolMap.end()){
//        symbolMap[tokens[traverseVec]]= notDeclared;
//    }
}

void pass1(Tokeniser *tokeniser){
    int globalOffset = 0;
    while(!tokeniser->eof()) {
        try {
            int defcount = stoi(tokeniser->getToken());
            for (int j = 0; j < defcount; j++) {
                readSym(tokeniser, globalOffset);
            }

            int usecount = stoi(tokeniser->getToken());
            for (int j = 0; j < usecount; j++) {
                readDecal(tokeniser);
            }

            int instcount = stoi(tokeniser->getToken());
            for (int j = 0; j < instcount; j++) {
                globalOffset++;
                string addressMode = tokeniser->getToken();
                int value = stoi(tokeniser->getToken());
            }
        }
        catch (...) {
            cout << parseError(3, 1, 1);
            exit(3);
        }
    }
}

void pass2(Tokeniser *tokeniser){
    int globalOffset = 0;
    int moduleBase = 0;
    int moduleNo = 1;
    while(!tokeniser->eof()) {
        int defcount = stoi(tokeniser->getToken());
        definedNotUsed.clear();
        for (int j = 0; j < defcount; j++) {
            definedNotUsed[tokeniser->getToken()] = true;
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
            checkDeclarationVec(declarationVec, memoryVec, moduleNo, definedNotUsed);
            addDefinedNotUsedWarning(definedNotUsed, warnings, moduleNo);
            moduleNo++;
        }
    }
}


int main(){
    string fileName = "D:\\NYU_assignment\\Spring_2020\\OS\\lab1samples\\input-4";

    Tokeniser tokeniser(fileName);

    pass1(&tokeniser);

    printSymbolTable(symbolMap);
    cout<<endl;

    // PASS 2
    tokeniser.reset();

    pass2(&tokeniser);

    printMemoryVector(memoryVec);
    cout<<endl;
//    printVector(warnings);
    cout<<endl<<endl;
    cout<<"end";
    return 0;
}
