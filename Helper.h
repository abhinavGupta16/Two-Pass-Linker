//
// Created by gupta on 07-02-2020.
//

#ifndef ASSIGNMENT1_HELPER_H
#define ASSIGNMENT1_HELPER_H
#include<iostream>
#include<string>
#include <bits/stdc++.h>

using namespace std;

void printSymbolTable(unordered_map<string, string> symbolMap, vector<string> symbolMapOrder);
string parseError(int errcode, int linenum, int lineoffset);
string errorMessages(int rule);
void processOperandE(int opcode, int operand, unordered_map<string, string> symbolMap, vector <pair<int, string>> &memoryVec, vector <pair<string, bool>> &declarationVec);
void printMemoryVector(vector <pair<int, string>> memoryVec);
void checkDeclarationVec(vector <pair<string, bool>> &declarationVec, vector <pair<int, string>> &memoryVec, int moduleNo, unordered_map<string, bool> &allDeclarationVec);
void addDefinedNotUsedWarning(unordered_map<string, int> definedNotUsed, vector <string> &warnings, vector<string> definedNotUsedOrder, unordered_map<string, bool> &allDeclarationVec);
void processImmediate(vector <pair<int, string>> &memoryVec, int value, int errorCode);
int convertToNum(string s);
void printVector(vector <string> vec);
string checkstring(string s);
string checkAddress(string s);
int checkInstCount(string s);
int checkUseCount(string s);
int checkDefCount(string s);
void checkForRule5(vector<pair<string, int>> origSymbolValuePair, vector <string> &warnings, int instCount, int moduleNo, unordered_map<string, string> &symbolMap);
#endif //ASSIGNMENT1_HELPER_H
