//
// Created by gupta on 07-02-2020.
//

#ifndef ASSIGNMENT1_HELPER_H
#define ASSIGNMENT1_HELPER_H
#include<iostream>
#include<string>
#include <bits/stdc++.h>

using namespace std;

void printSymbolTable(map<string, string> symbolMap);
string parseError(int errcode, int linenum, int lineoffset);
string errorMessages(int rule);
void processOperandE(int opcode, int operand, map<string, string> symbolMap, vector <pair<int, string>> &memoryVec, vector <pair<string, bool>> &declarationVec);
void printMemoryVector(vector <pair<int, string>> memoryVec);
void checkDeclarationVec(vector <pair<string, bool>> &declarationVec, vector <pair<int, string>> &memoryVec, int moduleNo, map<string, bool> definedNotUsed);
void addDefinedNotUsedWarning(map<string, bool> definedNotUsed, vector <string> &warnings, int moduleNo);
void processImmediate(vector <pair<int, string>> &memoryVec, int value, int errorCode);
#endif //ASSIGNMENT1_HELPER_H
