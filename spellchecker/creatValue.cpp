/*
	* creatValue
	* this class is for selecting feature words 
	* created by zhujieming 
	* copyright USTC
	* 22.05.2014
*/
#include "stdafx.h"
#include "creatValue.h"
#include "testWinnow.h"
#include <set>
#include <vector>
#include <string>
#include <cstring>
#include <map>
#include <math.h>
using namespace std;

creatValue::creatValue(void)
{
}

creatValue::~creatValue(void)
{
}

void creatValue::ig_filter(char* filename)
{	
}
void creatValue::load(char* fileName)
{
	const char* filepath = fileName;
	TiXmlDocument doc(filepath);
	bool loadOkay = doc.LoadFile();
	// faile to load 'phonebookdata.xml'.
	if (!loadOkay) {	
		printf( "Could not load test file %s. Error='%s'. Exiting.\n", filepath,doc.ErrorDesc() );
		exit( 1 );
	}
	TiXmlElement* root = doc.RootElement();
	int index = 0;
	for( TiXmlNode*  item = root->FirstChild( "CONFUSEWORDS" );
		 item;
	     item = item->NextSibling( "CONFUSEWORDS" ) ) 
	{
		set<string> word_set;
		for( TiXmlNode*  cword = item->FirstChild( "CONFUSEWORD" );
			 cword;
			 cword = cword->NextSibling( "CONFUSEWORD" ) )
		{
			TiXmlAttribute* cword_attrib = cword->ToElement()->FirstAttribute();
			string value = cword_attrib->Value();
			string word  = cword_attrib->Next()->Value();
			this->cnt[word] = atof(value.c_str());
			word_set.insert(word);
			map<string,float> fDict;
			for( TiXmlNode*  fword = cword->FirstChild( "FEATUREWORD" );
			     fword;
			     fword = fword->NextSibling( "FEATUREWORD" ) )
			{
				TiXmlAttribute* fword_attrib = fword->ToElement()->FirstAttribute();
				string word = fword_attrib->Value();
				const char* val = fword->ToElement()->GetText();
				fDict[word] = (float)atof(val);
			}
			this->featureword[word] = fDict;
		}
		this->confusewords.push_back(word_set);
	}
}
void creatValue::test()
{
	for (int i = 0; i < this->confusewords.size(); i++)
	{
		set<string> confusewords_set = this->confusewords[i];
		for(set<string>::iterator it =confusewords_set.begin(); it != confusewords_set.end(); it++)
		{
			string word = *it;
			cout<<word << " " <<this->cnt[word]<<endl;
			map<string , float> fw = this->featureword[word];
			for (map<string,float>::iterator f_it = fw.begin(); f_it != fw.end(); f_it++)
			{
				cout<<"   "<<f_it->first<<"   "<<f_it->second<<endl;
			}
		}
		cout<<"\n\n\n";
	}
}

char* c_str(string str)
{
	int len = str.length();
	char *ch = (char*)malloc(sizeof(char)*(len+1));
	for (int i=0;i < len; i++)
	{
		ch[i]=str[i];
	}
	ch[len] = '\0';
	return ch;
}

void creatValue::calculate()
{

	TiXmlDocument *pDoc=new TiXmlDocument;
    TiXmlDeclaration *pDeclaration=new TiXmlDeclaration("1.0","UTF-8","");
   	pDoc->LinkEndChild(pDeclaration);
	TiXmlElement *Confusesets = new TiXmlElement("CONFUSESETS");

	const float size_ratio = 20;
	int confusewords_size = confusewords.size();
	
	for (int i = 0; i < confusewords_size; i++)
	{
		set<string> cw_list = confusewords[i];
		int size_cw = cw_list.size();  // 混淆集的规模
		float size_fw = size_cw  *  size_ratio; // 混淆集的特征规模
		float total_stmt = 0;
		set<string> fw_set;
		for(set<string>::iterator it = cw_list.begin(); it != cw_list.end(); it++)
		{
			total_stmt += this->cnt[*it];
			map<string,float> fDict = this->featureword[*it];
			for (map<string,float>::iterator fDict_it = fDict.begin(); fDict_it != fDict.end(); fDict_it++)
			{
				fw_set.insert(fDict_it->first);
			}
		}
		map<string,float> fw_ig_dict;
		for (set<string>::iterator it = fw_set.begin(); it != fw_set.end(); it ++)
		{
			float ig = 0;
            float fw_count = 0;
			string f_word = *it;
			for(set<string>::iterator cw_list_it = cw_list.begin(); cw_list_it != cw_list.end(); cw_list_it++)
			{
				map<string,float> f_set = this->featureword[*cw_list_it];
				if (f_set.find(f_word) != f_set.end())
				{
					fw_count += f_set[f_word];
				}
			}
			float p_t = fw_count/total_stmt; // P(t)
			for(set<string>::iterator cw_list_it = cw_list.begin(); cw_list_it != cw_list.end(); cw_list_it++)
			{
				string word = *cw_list_it;
				float p_Ci = this->cnt[word]/total_stmt; // P(Ci)
				map<string,float> fw_list_x = this->featureword[word];
				float fw_Ci_count = 0;  // 同时含有Ci和fw的句子数,p(Ci*t)
				map<string,float> f_set = this->featureword[*cw_list_it];
				if (f_set.find(f_word) != f_set.end())
				{
					fw_Ci_count = f_set[f_word];
				}
				float p_Ci_t = fw_Ci_count/fw_count;  //      #P(Ci|t)
                float p_Ci_t_ = (p_Ci-fw_Ci_count/total_stmt)/(1-p_t); //#P(Ci|-t)
			    if (0 != p_Ci_t && 0 != p_Ci_t_ )
			    {
                    ig += p_t * p_Ci_t * (log(p_Ci_t)/log(2.0)) + (1-p_t) * p_Ci_t_ * (log(p_Ci_t_)/log(2.0))-p_Ci* (log(p_Ci)/log(2.0));
                    fw_ig_dict[f_word] = ig;
				}
				else
				{
                    fw_ig_dict[f_word] = 1;
                    break;
				}
			}
		}

		//统计最后选中的特征
        set<string> fw_set_final;
		for (set<string>::iterator it = fw_set.begin(); it != fw_set.end(); it ++)
		{
			string f_word = *it;
			if (1 != fw_ig_dict[f_word])  //公共特征
			{
				fw_set_final.insert(f_word);
				fw_ig_dict.erase(f_word);
			}
		}
		for (set<string>::iterator it = fw_set_final.begin(); it != fw_set_final.end(); it ++)
		{
			fw_set.erase(*it);
		}
		int remainder = size_fw - fw_set_final.size();

		//#从每个混淆词中提取相同个数的非公共特征
		for(set<string>::iterator cw_list_it = cw_list.begin(); cw_list_it != cw_list.end(); cw_list_it++)
		{
			int r = remainder/size_cw;
			map<string,float> fw_list = this->featureword[*cw_list_it];
			for (map<string,float>::iterator fw_list_it = fw_list.begin(); fw_list_it != fw_list.end(); fw_list_it++)
			{
				string f_word = fw_list_it->first;
				float cnt = fw_list_it->second;
				if (fw_ig_dict.find(f_word) != fw_ig_dict.end() && r>0)
				{
					fw_set_final.insert(f_word);
					fw_set.erase(f_word);
					fw_ig_dict.erase(f_word);
					r--;
				}
			}
		}
		//#从特征全集中补足不够的最终混淆集特征   
		if (fw_set_final.size() < size_fw)
		{
			int remainder=size_fw-fw_set_final.size();
			for (set<string>::iterator it = fw_set.begin(); it != fw_set.end(); it ++)
			{
				if (remainder>0)
				{
					fw_set_final.insert(*it);
					remainder--;
				}
			}
		}
	    TiXmlNode* Confusewords = new TiXmlElement("CONFUSEWORDS");
	    for (int i = 0; i < confusewords_size; i++)
    	{
			set<string> cw_list = confusewords[i];
			for(set<string>::iterator cw_list_it = cw_list.begin(); cw_list_it != cw_list.end(); cw_list_it++)
		    {
				string c_word = *cw_list_it;
				TiXmlNode* Confuseword = new TiXmlElement("CONFUSEWORD");
			    Confuseword->ToElement()->SetAttribute("name",c_word.c_str());
				for (set<string>::iterator fw_it = fw_set_final.begin(); fw_it != fw_set_final.end(); fw_it++)
				{
					TiXmlNode* Featureword = new TiXmlElement("FEATUREWORD");
					string set = "0.2";
					string word = *fw_it;
					Featureword->ToElement()->SetAttribute("name",c_str(word));
					Featureword->InsertEndChild(TiXmlText(set.c_str()));
					Confuseword->InsertEndChild(*Featureword);
				}
				Confusewords->InsertEndChild(*Confuseword);
		    }
		    Confusesets->InsertEndChild(*Confusewords);		
    	}
	}
    pDoc->InsertEndChild(*Confusesets);
	pDoc->SaveFile("withValue.xml");
}