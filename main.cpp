#include<iostream>
#include<string>
#include <bits/stdc++.h>
#include <map>
#include "Helper.h"

using namespace std;

int traverseVec;
map<string, string> symbolMap;
vector <pair<int, string>> memoryVec;
vector <string> warnings;
map<string, bool> definedNotUsed;
vector <pair<string, bool>> declarationVec;

//class Symbol{
//public:
//    string name;
//    int offset = -1;
//
//    Symbol (string n, int o) : name(n), offset(o){}
//
//    string getName() {
//        return ( name );
//    }
//
//    int getOffset() {
//        return ( offset );
//    }
//};

void readSym(vector <string> tokens, int globalOffset){
    int value;

    try{
        if(tokens[traverseVec].size()>16){
            throw 4;
        }
        if(traverseVec+1 > tokens.size()){
            throw 2;
        }
        value = stoi(tokens[traverseVec+1]);
    } catch (int e){
        cout<<parseError(e,1,1);
        exit(e);
    }
//    Symbol sys(tokens[traverseVec], value);

    if(symbolMap.find(tokens[traverseVec]) == symbolMap.end()) {//|| !symbolMap[tokens[traverseVec]].compare(notDeclared)){
        symbolMap[tokens[traverseVec]] = to_string(globalOffset + value);
    } else {
        symbolMap[tokens[traverseVec]] = symbolMap[tokens[traverseVec]] + " " + errorMessages(2);
    }
    traverseVec+=1;
}

void readDecal(vector <string> tokens){
      declarationVec.push_back(make_pair(tokens[traverseVec], false));
//    if(symbolMap.find(tokens[traverseVec]) == symbolMap.end()){
//        symbolMap[tokens[traverseVec]]= notDeclared;
//    }
}

void pass1(vector <string> tokens){
    int globalOffset = 0;
    for(traverseVec = 0; traverseVec < tokens.size();) {
        try{
            int defcount = stoi(tokens[traverseVec++]);
            for(int j = 0; j < defcount; j++) {
                readSym(tokens, globalOffset);
                traverseVec++;
            }


            int usecount = stoi(tokens[traverseVec++]);
            for(int j = 0; j < usecount; j++) {
                readDecal(tokens);
                traverseVec++;
            }

            int instcount = stoi(tokens[traverseVec++]);
            for (int j=0;j<instcount;j++) {
                globalOffset++;
                string addressMode = tokens[traverseVec++];
                int operand =  stoi(tokens[traverseVec])&1000;
                int opcode =  stoi(tokens[traverseVec++])/1000;
            }
        }
        catch (...) {
            cout<<parseError(3, 1, 1);
            exit(3);
        }
    }
}

void pass2(vector <string> tokens){
    int globalOffset = 0;
    int moduleBase = 0;
    int moduleNo = 1;
    for(traverseVec = 0; traverseVec < tokens.size();) {
        int defcount = stoi(tokens[traverseVec++]);
        definedNotUsed.clear();
        for(int j = 0; j < defcount; j++) {
            definedNotUsed[tokens[traverseVec++]] = true;
//          readSym(tokens, globalOffset);
            traverseVec++;
        }

        int usecount = stoi(tokens[traverseVec++]);
        declarationVec.clear();
        for(int j = 0; j < usecount; j++) {
            readDecal(tokens);
            traverseVec++;
        }

        int instcount = stoi(tokens[traverseVec++]);
        for (int j=0;j<instcount;j++) {
            string addressMode = tokens[traverseVec++];
            int value = stoi(tokens[traverseVec++]);
            int operand =  value%1000;
            int opcode =  value/1000;
            string errorMsg = "";
            bool flag = false;
            int errorCode = -1;
            if(opcode>=10){
                errorCode = 11;
                value = 9999;
                operand = value%1000;
                opcode =  value/1000;
                errorMsg = errorMessages(errorCode);
                flag = true;
            }
            if(!addressMode.compare("R")){
                if(operand >= instcount && !flag){
                    value = opcode*1000;
                    errorMsg = errorMessages(9);
                }
                memoryVec.push_back(make_pair(value + moduleBase, errorMsg));
            } else if(!addressMode.compare("E")){
                processOperandE(operand, opcode, symbolMap, memoryVec, declarationVec);
            } else if(!addressMode.compare("I")){
                processImmediate(memoryVec, value, errorCode);
            } else if(!addressMode.compare("A")){
                if(operand>=512 && !flag){
                    value = opcode*1000;
                    errorMsg = errorMessages(8);
                }
                memoryVec.push_back(make_pair(value, errorMsg));;
            }
            globalOffset++;
        }
        if(instcount>0) {
            moduleBase = globalOffset;
            checkDeclarationVec(declarationVec, memoryVec, moduleNo, definedNotUsed);
            addDefinedNotUsedWarning(definedNotUsed, warnings, moduleNo);
            moduleNo++;
        }
    }
}


int main(){

    // PASS 1
    vector <string> tokens;
    readFile(&tokens);

    pass1(tokens);

    printSymbolTable(symbolMap);
    cout<<endl;
    // PASS 2
    vector <string> tokens2;
    readFile(&tokens2);
    pass2(tokens2);

    printMemoryVector(memoryVec);
    cout<<endl;
//    printVector(warnings);
    cout<<endl<<endl;
    cout<<"end";
    return 0;
}
