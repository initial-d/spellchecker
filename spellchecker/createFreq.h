/*
	* createFreq
	* this class is for calculate the frequency of feature words 
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

class createFreq
{
public:
	createFreq();
	~createFreq();
	void createDel();
	void load_xml_file(char* fileName);
	void create();
private:
	map<string , int> deleteWords;
};