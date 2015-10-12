/*
	* Lexer
	* this class is to split scentence to words 
	* created by zhujieming 
	* copyright USTC
	* 22.05.2014
*/
#pragma once
#include <iostream>
#include <map>
#include <algorithm>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

class Lexer
{
public:
	Lexer(string stmt);
	~Lexer();
	vector<string> nextWord();
private:
	string Stmt;
	vector<string> ans;
};