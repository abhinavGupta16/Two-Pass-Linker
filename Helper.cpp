//
// Created by gupta on 07-02-2020.
//

#include "Helper.h"
#include<iostream>
#include<string>
#include <bits/stdc++.h>

using namespace std;

/*
 * Prints the warning Messages
 */
void printWarnings(vector <string> vec){
    for(int i = 0; i < vec.size(); i++) {
        cout << vec[i] << '\n';
    }
}

/*
 * Prints the Memory Vector
 */
void printMemoryVector(vector <pair<int, string>> memoryVec){
    int memoryValue = 0;
    cout<<"Memory Map"<<endl;
    for(int i = 0; i < memoryVec.size(); i++) {
        if(memoryVec[i].first==-1){
            cout<< memoryVec[i].second << endl;
        } else {
            cout << setw(3) << setfill('0') << memoryValue++;
            cout << ": " << setw(4) << setfill('0') << memoryVec[i].first << " " << memoryVec[i].second << endl;
        }
    }
}

/*
 * Prints the symbol Table
 */
void printSymbolTable(unordered_map<string, string> symbolMap, vector<string> symbolMapOrder) {
    cout<<"Symbol Table"<<endl;
    for(int i = 0; i< symbolMapOrder.size(); i++){
        cout << symbolMapOrder[i] << "=" << symbolMap[symbolMapOrder[i]] << "\n";
    }
}

/*
 * Returns the Error Message based on the Rule number
 */
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

/*
 * Returns the Parsing error based on the Rule number
 */
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

/*
 * Process the E instruction
 */
void processOperandE(int opcode, int operand, unordered_map<string, string> symbolMap, vector <pair<int, string>> &memoryVec, vector <pair<string, bool>> &declarationVec){
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

/*
 * Checks the use list for Rule 7 violation
 */
void checkDeclarationVec(vector <pair<string, bool>> &declarationVec, vector <pair<int, string>> &memoryVec, int moduleNo, unordered_map<string, bool> &allDeclarationVec){
    string warning = "\n";
    char buffer [150];
    bool flag = false;

    for(int i = 0; i < declarationVec.size(); i++) {
        allDeclarationVec[declarationVec[i].first] = declarationVec[i].second;
        if(!declarationVec[i].second){
            sprintf(buffer, errorMessages(7).c_str(), moduleNo, declarationVec[i].first.c_str());
            warning+= string(buffer);
            if(i < declarationVec.size() - 1){
                warning+="\n";
            }
            flag = true;
        }
    }

    if(flag){
        if(memoryVec.size()==0){
            memoryVec.push_back(make_pair(-1, warning.substr(1)));
        } else {
            memoryVec.back().second += warning;
        }
    }
}

/*
* Check for warnings for Rule 4 violation
*/
void addDefinedNotUsedWarning(unordered_map<string, int> definedNotUsed, vector <string> &warnings, vector<string> definedNotUsedOrder, unordered_map<string, bool> &allDeclarationVec){
    char buffer[150];
    for(int i = 0; i < definedNotUsedOrder.size(); i++){
        string symbol = definedNotUsedOrder[i];
        if(allDeclarationVec.find(symbol) == allDeclarationVec.end() || !allDeclarationVec[symbol]) {
            sprintf(buffer, errorMessages(4).c_str(), definedNotUsed[symbol], symbol.c_str());
            warnings.push_back(string(buffer));
        }
    }
}

/*
* Process the Instruction I
*/
void processImmediate(vector <pair<int, string>> &memoryVec, int value, int errorCode){
    string errorMsg = "";
    if(value >=10000){
        errorCode = 10;
        value = 9999;
    }
    if(errorCode != -1){
        errorMsg = errorMessages(errorCode);
    }
    memoryVec.push_back(make_pair(value, errorMsg));
}

/*
* Check parse error and convert string to num
*/
int convertToNum(string s){
    try{
        return stoi(s);
    } catch(...){
        throw 1;
    }
}

/*
* Check if a value is a number
*/
bool isNumber(string s)
{
    for (int i = 0; i < s.length(); i++)
        if (isdigit(s[i]) == false)
            return false;

    return true;
}

/*
* Check parse error expecting string
*/
string checkstring(string s){
    if(isNumber(s)){
        throw 2;
    } else {
        return s;
    }
}

/*
* Check for Instruction parse error
*/
string checkAddress(string s){
    if (s=="R" || s=="A" || s=="I" || s=="E") {
        return s;
    } else {
        throw 3;
    }
}

/*
* Parse error too many instructions
*/
int checkInstCount(string s, int &totalInstCount){
    int num = convertToNum(s);
    totalInstCount+=num;
    if(totalInstCount>512){
        throw 7;
    }
    return num;
}

/*
* Parse error too many variables in Use List
*/
int checkUseCount(string s){
    int num = convertToNum(s);
    if(num>16){
        throw 6;
    }
    return num;
}

/*
* Parse error too many variables in Definition List
*/
int checkDefCount(string s){
    int num = convertToNum(s);
    if(num>16){
        throw 5;
    }
    return num;
}

/*
* Check error for rule 5
*/
void checkForRule5(vector<pair<string, int>> origSymbolValuePair, vector <string> &warnings, int instCount, int moduleNo, unordered_map<string, string> &symbolMap){
    char buffer[150];
    for(int i = 0; i < origSymbolValuePair.size(); i++){
        string symbol = origSymbolValuePair[i].first;
        int value = origSymbolValuePair[i].second;
        if(instCount <= value){
            symbolMap[symbol] = to_string(stoi(symbolMap[symbol]) - value);
            sprintf(buffer, errorMessages(5).c_str(), moduleNo, symbol.c_str(), value, instCount-1);
            warnings.push_back(string(buffer));
        }
    }
}
