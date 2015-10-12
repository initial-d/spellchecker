/*
	* loadDirect
	* this class is to write preposition to xml file 
	* created by zhujieming 
	* copyright USTC
	* 22.05.2014
*/
#include "loadDirect.h"
#include<sstream>
#include<vector>

loadDirect::loadDirect()
{
}
loadDirect::~loadDirect()
{
}
void loadDirect::createDirect(map<string , vector<string>>& dict , map<string , int>& condictflag)
{
	ifstream fin ("directcon.txt");
	int temp = 0;
	char line[100];
	while(fin.getline(line , 100))
	{
		string tmpline(line);
		stringstream ss(tmpline);
		if(tmpline.size() == 0)
		{
			//cout<<endl;
			temp = 0;
		}
		else
		{
			string sstr , confu;
			ss>>confu;
			while(ss>>sstr)
			{
				dict[confu].push_back(sstr);
				condictflag[confu] = 1;
			//	cout<<sstr<<endl;
			}
			temp++;
		}
	}
	/*
	for(map<string , vector<string>>::iterator it = dict.begin() ; it != dict.end(); it++)
	{
		cout<<it->first<<" ";
		for(int i = 0 ; i < dict[it->first].size() ; i++)
		{
			cout<<dict[it->first][i]<<" ";
		}
		cout<<endl;
	}*/
}