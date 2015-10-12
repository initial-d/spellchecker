/*
	* Winnow
	* this class is to realize the winnow algorithm
	* created by duyimin 
	* copyright USTC
	* 22.05.2014
*/
#include "winnow.h"
#include <sstream>
#include <fstream>
#include "Lexer.h"

Winnow::Winnow()
{
	alpha = 1.3;
	beta = 0.8;
	theta = 1.1;
}
Winnow::~Winnow()
{
}
void Winnow::decreaseValue(map<string , float>& rDict ,map<string , map<string , float>>& valueDict, string one)
{
	map<string , float> tmpDict = valueDict[one];
	for(map<string , float>::iterator it = rDict.begin(); it != rDict.end(); it++)
	{
		tmpDict[it->first] = it->second * beta;
	}
	valueDict[one] = tmpDict;
}
void Winnow::increaseValue(map<string , float>& rDict ,map<string , map<string , float>>& valueDict, string one)
{
	map<string , float> tmpDict = valueDict[one];
	for(map<string , float>::iterator it = rDict.begin(); it != rDict.end(); it++)
	{
		tmpDict[it->first] = it->second * alpha;
	}
	valueDict[one] = tmpDict;
}

void Winnow::execise(map<string , map<string , float>>& valueDict , map<string , vector<string>>& oneToMany ,map<string , vector<string>>& stmtDict )
{
	for(map<string , vector<string>>::iterator it = stmtDict.begin(); it != stmtDict.end(); it++)
	{
		vector<string> manyList = oneToMany[it->first];
		doExecise(it->first , it->second , manyList , valueDict);
	}
}

void Winnow::doExecise(string word , vector<string> &sList , vector<string> &confuseSet ,map<string , map<string , float>>& valueDict)
{
	for(int i = 0 ; i < sList.size() ; i++)
	{
		map<string , map<string , float> > resultDict;
		vector<string> stAfterTag;
		map<string , float> result;
		map<string , float> sumDict;
		map<string , float> oneSum;
		stAfterTag = tagStmt(sList[i]);
		for(int j = 0 ; j < confuseSet.size(); j++)
		{
			result = calValue(confuseSet[j] , valueDict , stAfterTag);
			resultDict[confuseSet[j]] = result;
		}
		for(int j = 0 ; j < confuseSet.size(); j++)
		{
			oneSum = resultDict[confuseSet[j]];
			float oSum = 0;
			for(map<string , float>::iterator it = oneSum.begin(); it != oneSum.end(); it++)
			{
				oSum = oSum + it->second;
			}
			sumDict[confuseSet[j]] = oSum;
		}
		for(int j = 0 ; j < confuseSet.size(); j++)
		{
			if(sumDict[confuseSet[j]] > theta && confuseSet[j] != word)
			{
				decreaseValue(resultDict[confuseSet[j]],valueDict,confuseSet[j]);
			}
			if(sumDict[confuseSet[j]] < theta && confuseSet[j] == word)
			{
				increaseValue(resultDict[confuseSet[j]],valueDict,confuseSet[j]);
			}
		}
	}
}

vector<string> Winnow::tagStmt(string stmt)
{
	vector<string> temp;
	Lexer lexer(stmt);
	temp = lexer.nextWord();
	return temp;
}
map<string , float> Winnow::calValue(string word , map<string , map<string , float>>& valueDict , vector<string>& stAfterTag)
{
	map<string , float> featureWordDict;
	featureWordDict = valueDict[word];
	map<string , float> exsitDict;
	for(int i = 0 ; i < stAfterTag.size() ; i++)
	{
		if(word == stAfterTag[i])
		{
			string conjunction;
			vector<string> conv;
			if(i < stAfterTag.size() - 1)
			{
				conjunction = stAfterTag[i + 1];
				if(prep[conjunction])
				{
					confusePrep[word].insert(conjunction);	
				}
			}
		}

		if(featureWordDict[stAfterTag[i]])
		{
			exsitDict[stAfterTag[i]] = featureWordDict[stAfterTag[i]];
		}
	}
	return exsitDict;
}

void  Winnow::saveDict(map<string , map<string , float>>& valueDict ,map<string , vector<string>>& oneToMany)
{
	map<string , int> eSet;
	vector<vector<string> > wordsSet;
	for(map<string , vector<string> >::iterator it = oneToMany.begin(); it != oneToMany.end(); it++)
	{	
		//for(int i = 0 ; i < oneToMany[it->first].size() ; i++)
		//	cout<<oneToMany[it->first][i]<<" ";
		//cout<<endl;
		if(!eSet[it->first])
		{
			wordsSet.push_back(it->second);
			for(int i = 0 ; i < oneToMany[it->first].size() ; i++)
				eSet[oneToMany[it->first][i]] = 1;
		}
	}
	//cout<<wordsSet.size()<<endl;
	TiXmlDocument *pDoc=new TiXmlDocument;
	TiXmlDeclaration *pDeclaration=new TiXmlDeclaration("1.0","UTF-8","");
	pDoc->LinkEndChild(pDeclaration);
    TiXmlElement *confusesets = new TiXmlElement("CONFUSESETS");
	for(int j = 0 ; j < wordsSet.size() ; j++)
	{
		TiXmlNode* newNode = new TiXmlElement("CONFUSEWORDS");
		vector<string> item = wordsSet[j];
		for(int i = 0 ; i < item.size() ; i++)
		{
			TiXmlNode* word = new TiXmlElement("CONFUSEWORD");
			word->ToElement()->SetAttribute("name",item[i].c_str());
			map<string , float> oneDict = valueDict[item[i]];
			for(map<string , float>::iterator it = oneDict.begin(); it != oneDict.end(); it++)
			{	
				TiXmlNode* featureWord = new TiXmlElement("FEATUREWORD");
				featureWord->ToElement()->SetAttribute("name",it->first.c_str());
				ostringstream oss;
				oss<<it->second;	
				string str(oss.str());
				featureWord->InsertEndChild(TiXmlText(str.c_str()));
				word->InsertEndChild(*featureWord);
			}
			newNode->InsertEndChild(*word);
		}
		confusesets->InsertEndChild(*newNode);
	}
	pDoc->InsertEndChild(*confusesets);
	pDoc->SaveFile("afterUpdate.xml");
}

void Winnow::addPrep(map<string , int>& prep)
{
	prep["via"] = 1;
	prep["behind"] = 1;
	prep["beyond"] = 1;
	prep["round"] = 1;
	prep["around"] = 1;
	prep["aboard"] = 1;
	prep["toward"] = 1;
	prep["since"] = 1;
	prep["beside"] = 1;
	prep["inside"] = 1;
	prep["outside"] = 1;
	prep["like"] = 1;
	prep["unlike"] = 1;
	prep["before"] = 1;
	prep["despite"] = 1;
	prep["opposite"] = 1;
	prep["above"] = 1;
	prep["of"] = 1;
	prep["off"] = 1;
	prep["concerning"] = 1;
	prep["during"] = 1;
	prep["along"] = 1;
	prep["belong"] = 1;
	prep["among"] = 1;
	prep["through"] = 1;
	prep["beneath"] = 1;
	prep["with"] = 1;
	prep["until"] = 1;
	prep["till"] = 1;
	prep["from"] = 1;
	prep["between"] = 1;
	prep["in"] = 1;
	prep["within"] = 1;
	prep["on"] = 1;
	prep["upon"] = 1;
	prep["down"] = 1;
	prep["to"] = 1;
	prep["into"] = 1;
	prep["onto"] = 1;
	prep["up"] = 1;
	prep["near"] = 1;
	prep["under"] = 1;
	prep["per"] = 1;
	prep["after"] = 1;
	prep["over"] = 1;
	prep["for"] = 1;
	prep["as"] = 1;
	prep["towards"] = 1;
	prep["besides"] = 1;
	prep["across"] = 1;
	prep["plus"] = 1;
	prep["minus"] = 1;
	prep["versus"] = 1;
	prep["at"] = 1;
	prep["except"] = 1;
	prep["past"] = 1;
	prep["against"] = 1;
	prep["but"] = 1;
	prep["without"] = 1;
	prep["about"] = 1;
	prep["throughout"] = 1;
	prep["below"] = 1;
	prep["by"] = 1;
}

void Winnow::calPrep( map<string , vector<string>>& oneToMany) //”–÷ÿ∏¥º∆À„£°
{
	map<string , int> Visword;
	for(map<string , vector<string>>::iterator it = oneToMany.begin(); it != oneToMany.end(); it++)
	{
		if(Visword[it->first])
			continue;
		vector<string> manyList = oneToMany[it->first];
		map<string , int> Countp ;
		for(int i = 0 ; i < manyList.size() ; i++)
		{
			Visword[manyList[i]] = 1;
			for(set<string>::iterator ss = confusePrep[manyList[i]].begin() ; ss != confusePrep[manyList[i]].end() ; ss++)
			{
				Countp[*ss]++;
			}
		}
		for(int i = 0 ; i < manyList.size() ; i++)
		{
			for(set<string>::iterator ss = confusePrep[manyList[i]].begin() ; ss != confusePrep[manyList[i]].end() ; )
			{
				if(Countp[*ss] > 1)
					confusePrep[manyList[i]].erase(ss++);
				else
					ss++;
			}
		}
	}
}
void Winnow::writeP(map<string , vector<string>>& oneToMany)
{
	ofstream fout ("directcon.txt");
	map<string , int> Visword;
	for(map<string , vector<string>>::iterator it = oneToMany.begin(); it != oneToMany.end(); it++)
	{
		if(Visword[it->first])
			continue;
		vector<string> manyList = oneToMany[it->first];
		for(int i = 0 ; i < manyList.size() ; i++)
		{
			Visword[manyList[i]] = 1;
			fout<<manyList[i]<<" ";
			for(set<string>::iterator ss = confusePrep[manyList[i]].begin() ; ss != confusePrep[manyList[i]].end() ; ss++)
			{
				fout<<*ss<<" ";
			}
			fout<<endl;
		}
		fout<<endl;
	}
	fout.close();
}

void Winnow::winnow()
{
	map<string , map<string , float>> valueDict ;
	map<string , vector<string>> oneToMany;
	map<string , vector<string>> stmtDict;
	addPrep(prep);
	filetools testW;
	testW.load_inputXML_file("withValue.xml",valueDict,oneToMany);
	testW.load_stmtXML_file("stmt.xml",stmtDict);
	execise(valueDict,oneToMany,stmtDict);
	calPrep(oneToMany);
	writeP(oneToMany);
	saveDict(valueDict,oneToMany);
}