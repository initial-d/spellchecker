/*
	* Lexer
	* this class is to split scentence to words 
	* created by zhujieming 
	* copyright USTC
	* 22.05.2014
*/
#include "Lexer.h"

Lexer::Lexer(string stmt)
{
	Stmt = stmt;
}
vector<string> Lexer::nextWord()
{
	char str[30];
	int len = 0;
	memset(str , '\0' ,sizeof(str));
	for(int i = 0 ; i < Stmt.size() ; i++)
	{
		if(Stmt[i] >= 'a' &&Stmt[i] <= 'z' || Stmt[i] >= 'A' &&Stmt[i] <= 'Z')
		{
			if(Stmt[i] == 'n' && Stmt[i + 1] == '\'' && Stmt[i + 2] == 't')
			{
				str[len++] = Stmt[i];
				str[len++] = Stmt[++i];
				str[len++] = Stmt[++i];
				string temp1(str);
				ans.push_back(temp1);
				memset(str , '\0' ,sizeof(str));
				len = 0;
				continue;
			}
			else
				str[len++] = tolower(Stmt[i]);
		}
		else if(len)
		{
			string temp(str);
			if(len > 1 || temp[0] == 'a' || temp[0] == 'I')
				ans.push_back(temp);
			memset(str , '\0' ,sizeof(str));
			len = 0;
			if(Stmt[i] == '\'')
			{
				if(Stmt[i + 1] == 's')
				{
					str[len++] = Stmt[i];
					str[len++] = Stmt[++i];
					string temp2(str);
					ans.push_back(temp2);
					memset(str , '\0' ,sizeof(str));
					len = 0;
					continue;
				}
			}
			continue;
		}
	}
	return ans;
}

Lexer::~Lexer()
{
}