/*
	* filetools
	* this class is to definit some useful funtions
	* created by yangziqiang
	* copyright USTC
	* 22.05.2014
*/
#pragma once
#include "tinyxml.h"
#include <iostream>
#include <map>
#include <algorithm>
#include <string>
#include <vector>
using namespace std;
class filetools
{
public:
	filetools();
	~filetools();
	void load_inputXML_file(char* fileName , map<string , map<string , float>> &valueDict , map<string , vector<string>> &oneToMany);
	void load_stmtXML_file(char *fileName , map<string , vector<string>> &stmtDict , float percent = 0.8);
};