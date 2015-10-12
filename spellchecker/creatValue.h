/*
	* creatValue
	* this class is for selecting feature words 
	* created by zhujieming 
	* copyright USTC
	* 22.05.2014
*/
#pragma once
#include <set>
#include <vector>
#include <string>
#include <map>
using namespace std;
class creatValue
{
private:
	vector<set<string>> confusewords;
	map<string,float> cnt;
	map<string,map<string,float>> featureword;
public:
	void load(char* file_name);
	void ig_filter(char* fileName);
	void calculate();
	void test();
	creatValue(void);
	~creatValue(void);
};

