/*
	* Winnow
	* this class is to realize the winnow algorithm
	* created by duyimin 
	* copyright USTC
	* 22.05.2014
*/
#include "tinyxml.h"
#include <iostream>
#include <map>
#include <algorithm>
#include <string>
#include <vector>
#include <set>
#include <fstream>
#include "tools.h"
#include <set>
using namespace std;
class Winnow
{
public :
	Winnow();
	~Winnow();
	void winnow();
	void decreaseValue(map<string , float>& rDict ,map<string , map<string , float>>& valueDict, string one);
	void increaseValue(map<string , float>& rDict ,map<string , map<string , float>>& valueDict, string one);
	vector<string> tagStmt(string stmt);
	map<string , float> calValue(string word , map<string , map<string , float>>& valueDict , vector<string>& stAfterTag);
	void execise(map<string , map<string , float>>& valueDict , map<string , vector<string>>& oneToMany ,map<string , vector<string>>& stmtDict );
	void doExecise(string word , vector<string> &sList , vector<string> &confuseSet ,map<string , map<string , float>>& valueDict);
	void saveDict(map<string , map<string , float>>& valueDict ,map<string , vector<string>>& oneToMany);
	void addPrep(map<string , int>& prep);
	void calPrep(map<string , vector<string>>& oneToMany);
	void writeP(map<string , vector<string>>& oneToMany);
private:
	float alpha;
	float beta;
	float theta;
	map<string , int> prep;
	map<string , set<string> > confusePrep;
};
