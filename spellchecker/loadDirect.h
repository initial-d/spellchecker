/*
	* loadDirect
	* this class is to write preposition to xml file 
	* created by zhujieming 
	* copyright USTC
	* 22.05.2014
*/
#include "tinyxml.h"
#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>
#include <string>
#include <vector>
using namespace std;

class loadDirect
{
public:
	loadDirect();
	~loadDirect();
	void createDirect(map<string , vector<string>>& dict , map<string , int>& condictflag);
};