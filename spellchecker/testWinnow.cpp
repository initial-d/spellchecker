/*
	* testWinnow
	* this class is to test the results of winnow algorithm
	* created by duyimin 
	* copyright USTC
	* 22.05.2014
*/
#include "testWinnow.h"
#include "Lexer.h"
#include <sstream>
#include "loadDirect.h"

int cmp(const PAIR& x, const PAIR& y)
{
	return x.second > y.second; 
}
void testWinnow::test(float t)
{
	theta = t;
	ofstream fout ("test.txt");
	map<string , map<string , float>> valueDict ;
	map<string , vector<string>> oneToMany;
	map<string , vector<string>> stmtDict;
	filetools testW;
	testW.load_inputXML_file("afterUpdate.xml",valueDict,oneToMany);
	testW.load_stmtXML_file("stmt.xml",stmtDict,0.2);
	loadDirect loaddd;
	loaddd.createDirect(condict , condictflag);
	doTest(valueDict,oneToMany,stmtDict,fout);
	createError();
}

void testWinnow::doTest(map<string , map<string , float>>& valueDict , map<string , vector<string>>& oneToMany , map<string , vector<string>>& stmtDict ,ofstream& fout)
{
	set<vector<string>> setS;
	int select ,correct, total;
	float acc, re, f1;
	int Allword = 0;
	float Allcorr = 0.0;
	float Allre = 0.0;
	float Allf1 = 0.0;
	for(map<string , vector<string>>::iterator it = oneToMany.begin(); it != oneToMany.end(); it++)
	{
		setS.insert(it->second);
	}
	for(set<vector<string>>::iterator it = setS.begin(); it != setS.end(); it++)
	{
		vector<string> tmpList = *it;
		map<string ,vector<int> > templ;
		map<string ,int> miss;
		for(int i = 0 ; i < tmpList.size(); i++)
		{
			testConfuseSet(tmpList[i] , stmtDict[tmpList[i]], tmpList , valueDict , templ ,miss, fout);
		}
		for(int i = 0 ; i < tmpList.size(); i++)
		{
			select = templ[tmpList[i]][1] + miss[tmpList[i]];
			correct = templ[tmpList[i]][1];
			total = templ[tmpList[i]][0];
			if(select == 0)
				acc = -1;
			else
				acc = (float)correct / (float)select;
			if(total == 0)
				re = -1;
			else
				re = (float)correct / (float)total;
			if(acc + re == 0)
				f1 = -1;
			else
				f1 = (float)(acc * re * 2) / (float)(acc+re);
			Allcorr += acc;
			Allword++;
			Allre += re;
			Allf1 += f1;
			fout<<"  Word "+tmpList[i]+" 准确率: ";
			fout<<acc;
			fout<<"  召回率: ";
			fout<<re;
			fout<<"  F1测度: ";
			fout<<f1<<endl;
		}
		fout<<endl<<endl;
	}
	fout<<endl<<"  Theta = ";
	fout<<theta;
	fout<<"\n  Total 准确率: ";
	fout<<(float)Allcorr/(float)Allword;
    fout<<"  召回率: ";
	fout<<(float)Allre/(float)Allword;
	fout<<"  F1测度: ";
	fout<<(float)Allf1/Allword<<endl<<endl;
	fout<<"--------------------------------------------------------------------"<<endl<<endl;
}

void testWinnow::testConfuseSet(string word, vector<string>& sList, vector<string>& confuseSet , map<string , map<string , float>>& valueDict , map<string ,vector<int> >& templ ,map<string ,int>& miss ,ofstream& fout)
{
	int correct = 0;
	int allStmt = 0;
	int Miss = 0;
	vector<string> errorList;
	for(int i = 0 ; i < sList.size(); i++)
	{
		map<string , map<string , float>> resultDict;
		map<string , float> result;
		map<string , float> sumDict;
		map<string , float> oneSum;
		vector<string> stAfterTag;
		vector<PAIR> vec; 
		allStmt = allStmt + 1;
		stAfterTag = tagStmt(sList[i]) ;
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
		for(map<string , float>::iterator it = sumDict.begin(); it != sumDict.end(); it++)
		{
			vec.push_back(make_pair(it->first, it->second));  
		}
		sort(vec.begin(), vec.end(), cmp); 
		if(vec[0].first == word)
			correct = correct + 1;
		else
		{
			miss[vec[0].first]++;
			Miss = Miss + 1;
			errorList.push_back(sList[i]);
		}
	}
	templ[word].push_back(allStmt);
	templ[word].push_back(correct);
	errorDict[word] = errorList;
	fout<<"  "+word+" Total: ";
	fout<<allStmt;
	fout<<"  Correct: ";
	fout<<correct;
	fout<<" Miss: ";
	fout<<Miss<<endl;
	AllStmt = AllStmt+allStmt;
    AllCorre = AllCorre+correct;
}

vector<string> testWinnow::tagStmt(string stmt)
{
	vector<string> temp;
	Lexer lexer(stmt);
	temp = lexer.nextWord();
	return temp;
}
map<string , float> testWinnow::calValue(string word , map<string , map<string , float>>& valueDict , vector<string>& stAfterTag)
{
	map<string , float> featureWordDict;
	featureWordDict = valueDict[word];
	map<string , float> exsitDict;
	bool flag = false;
	for(int i = 0 ; i < stAfterTag.size() ; i++)
	{
		if(word == stAfterTag[i])
		{
			string conjunction;
			vector<string> conv;
			if(i < stAfterTag.size() - 1)
			{
				conjunction = stAfterTag[i + 1];
				if(condictflag[word])
				{
					conv = condict[word];
					for(int g = 0 ; g < conv.size() ;g++)
					{
						if(conjunction == conv[g])
						{
							cout<<word<<" "<<conjunction<<endl;
							flag = true;
						}
					}
				}
			}
		}
		if(featureWordDict[stAfterTag[i]])
		{
			exsitDict[stAfterTag[i]] = featureWordDict[stAfterTag[i]];
		}
	}
	if(flag){
		for(map<string , float>::iterator it = exsitDict.begin() ; it != exsitDict.end(); it++)
		{
			it->second += 100.0; 
		}
	}
	return exsitDict;
}
void testWinnow::createError()
{
	char line[100];
	TiXmlDocument *pDoc=new TiXmlDocument;
	TiXmlDeclaration *pDeclaration=new TiXmlDeclaration("1.0","UTF-8","");
	pDoc->LinkEndChild(pDeclaration);
    TiXmlElement *confusesets = new TiXmlElement("CONFUSESETS");
	TiXmlNode* newNode = new TiXmlElement("CONFUSESET");
	ifstream fin ("words.txt");
	int temp = 0;
	while(fin.getline(line , 100))
	{
		string tmpline(line);
		if(tmpline.size() == 0)
		{
			newNode->ToElement()->SetAttribute("size",temp);
			confusesets->InsertEndChild(*newNode);
			newNode = new TiXmlElement("CONFUSESET");
			//cout<<endl;
			temp = 0;
		}
		else
		{
			//cout<<tmpline<<endl;
			TiXmlNode* name4 = new TiXmlElement("WORD");
			name4->ToElement()->SetAttribute("size",errorDict[tmpline].size());
			name4->ToElement()->SetAttribute("name",tmpline.c_str());
			for(int i = 0 ; i < errorDict[tmpline].size() ; i++)
			{
				TiXmlNode* name3 = new TiXmlElement("ERROR");
				name3->InsertEndChild(TiXmlText(errorDict[tmpline][i].c_str()));
				name4->InsertEndChild(*name3);
			}
			newNode->InsertEndChild(*name4);
			temp++;
		}
	}
	pDoc->InsertEndChild(*confusesets);
	pDoc->SaveFile("errorList.xml");
}

testWinnow::testWinnow()
{
}
testWinnow::~testWinnow()
{
}