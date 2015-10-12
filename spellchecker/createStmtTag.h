/*
	* createStmtTag
	* this class is for split scentence to words  
	* created by lijunbao 
	* copyright USTC
	* 22.05.2014
*/
#include "tinyxml.h"
#include <iostream>
#include <map>
#include <algorithm>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

class createStmtTag
{
public:
	createStmtTag();
	~createStmtTag();
	vector<string> tagStmt(string stmt);
	void load_xml_file(char* fileName);
	void create();
};