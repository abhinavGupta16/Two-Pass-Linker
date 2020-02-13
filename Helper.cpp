//
// Created by gupta on 07-02-2020.
//

#include "Helper.h"
#include<iostream>
#include<string>
#include <bits/stdc++.h>

using namespace std;


void printVector(vector <string> vec){
    // Printing the vector
    for(int i = 0; i < vec.size(); i++) {
        cout << vec[i] << '\n';
    }
}

void printMemoryVector(vector <pair<int, string>> memoryVec){
    int memoryValue = 0;
    cout<<"Memory Map"<<endl;
    for(int i = 0; i < memoryVec.size(); i++) {
        cout << setw(3) << setfill('0') << memoryValue++;
        cout << ": " << setw(4) << setfill('0') << memoryVec[i].first << " " << memoryVec[i].second << endl;
    }
}

void printSymbolTable(map<string, string> symbolMap) {
    cout<<"Symbol Table"<<endl;
    for(auto elem : symbolMap) {
        cout << elem.first << "=" << elem.second << "\n";
    }
}

string errorMessages(int rule){
    string errorstring;
    switch (rule){
        case 2: errorstring = "Error: This variable is multiple times defined; first value used";
            break;
        case 3: errorstring = "Error: %s is not defined; zero used";
            break;
        case 4: errorstring = "Warning: Module %d: %s was defined but never used";
            break;
        case 5: errorstring = "Warning: Module %d: %s too big %d (max=%d) assume zero relative";
            break;
        case 6: errorstring = "Error: External address exceeds length of uselist; treated as immediate";
            break;
        case 7: errorstring = "Warning: Module %d: %s appeared in the uselist but was not actually used";
            break;
        case 8: errorstring = "Error: Absolute address exceeds machine size; zero used";
            break;
        case 9: errorstring = "Error: Relative address exceeds module size; zero used";
            break;
        case 10: errorstring = "Error: Illegal immediate value; treated as 9999";
            break;
        case 11: errorstring = "Error: Illegal opcode; treated as 9999";
            break;
    }
    return errorstring;
}

string parseError(int errcode, int linenum, int lineoffset) {
    char buffer[150];
    string errorString;
    switch (errcode){
        case 1: errorString = "NUM_EXPECTED";   // Number expect
            break;
        case 2: errorString = "SYM_EXPECTED";   // Symbol Expected
            break;
        case 3: errorString = "ADDR_EXPECTED";  // Addressing Expected which is A/E/I/R
            break;
        case 4: errorString = "SYM_TOO_LONG";   // Symbol Name is too long
            break;
        case 5: errorString = "TOO_MANY_DEF_IN_MODULE"; // > 16
            break;
        case 6: errorString = "TOO_MANY_USE_IN_MODULE"; // > 16
            break;
        case 7: errorString = "TOO_MANY_INSTR"; // total num_instr exceeds memory size (512)
            break;
    }
    sprintf(buffer, "Parse Error line %d offset %d: %s\n", linenum, lineoffset, errorString.c_str());
    return string(buffer);
}

void processOperandE(int opcode, int operand, map<string, string> symbolMap, vector <pair<int, string>> &memoryVec, vector <pair<string, bool>> &declarationVec){
    if(opcode>=declarationVec.size()){
        processImmediate(memoryVec, operand*1000 + opcode, 6);
        return;
    }
    char buffer [150];
    string symbol = declarationVec[opcode].first;
    declarationVec[opcode].second = true;
    int symbolValue = 0;
    string warning = "";
    if(symbolMap.find(symbol) == symbolMap.end()){
        sprintf(buffer, errorMessages(3).c_str(), symbol.c_str());
        warning += string(buffer);
    } else {
        symbolValue = stoi(symbolMap[symbol]);
    }
    int eValue = operand*1000 + symbolValue;
    memoryVec.push_back(make_pair(eValue, warning));
}

void checkDeclarationVec(vector <pair<string, bool>> &declarationVec, vector <pair<int, string>> &memoryVec, int moduleNo, map<string, bool> definedNotUsed){
    string warning = "";
    warning+="\n";
    char buffer [150];
    bool flag = false;
    for(int i = 0; i < declarationVec.size(); i++) {
        if(!declarationVec[i].second){
            if(definedNotUsed.find(declarationVec[i].first) == definedNotUsed.end()){
                definedNotUsed[declarationVec[i].first] = false;
            }
            if(flag){
                warning += "\n";
            }
            sprintf(buffer, errorMessages(7).c_str(), moduleNo, declarationVec[i].first.c_str());
            warning += string(buffer);
            flag = true;
        }
    }
    if(flag){
        memoryVec.back().second += warning;
    }
}

void addDefinedNotUsedWarning(map<string, bool> definedNotUsed, vector <string> &warnings, int moduleNo){
    map<string, bool>::iterator it = definedNotUsed.begin();
    char buffer[150];
    // Iterate over the map using Iterator till end.
    while (it != definedNotUsed.end()) {
        string warning = errorMessages(4);
        string symbol = it->first;
        if(it->second){
            sprintf(buffer, errorMessages(4).c_str(), moduleNo, symbol.c_str());
            warning += string(buffer);
            warnings.push_back(warning);
        }
        it++;
    }
}

void processImmediate(vector <pair<int, string>> &memoryVec, int value, int errorCode){
    string errorMsg = "";
    if(value >=10000){
        errorCode = 10;
    }
    if(errorCode != -1){
        errorMsg = errorMessages(errorCode);
    }
    memoryVec.push_back(make_pair(value, errorMsg));
}