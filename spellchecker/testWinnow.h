/*
	* testWinnow
	* this class is to test the results of winnow algorithm
	* created by duyimin 
	* copyright USTC
	* 22.05.2014
*/
#include "tinyxml.h"
#include <iostream>
#include <map>
#include <algorithm>
#include <string>
#include <string.h>
#include <vector>
#include <fstream>
#include "tools.h"
#include <set>
using namespace std;
typedef pair<string, float> PAIR; 
class testWinnow
{
public :
	testWinnow();
	~testWinnow();
	void test(float t);
	vector<string> tagStmt(string stmt);
	map<string , float> calValue(string word , map<string , map<string , float>>& valueDict , vector<string>& stAfterTag);
	void doTest(map<string , map<string , float>>& valueDict , map<string , vector<string>>& oneToMany , map<string , vector<string>>& stmtDict ,ofstream& fout);
	void testConfuseSet(string word, vector<string>& sList, vector<string>& confuseSet , map<string , map<string , float>>& valueDict ,map<string , vector<int> >& templ,map<string ,int>& miss , ofstream& fout);
	void createError();
	//int cmp(const PAIR& x, const PAIR& y);
private:
	float theta;
	map<string , vector<string>> errorDict;
	map<string , vector<string>> condict;
	map<string , int> condictflag;
	int AllStmt;
	int AllCorre;
};