/*
	* createFreq
	* this class is for calculate the frequency of feature words 
	* created by lijunbao 
	* copyright USTC
	* 22.05.2014
*/
#include "createFreq.h"
#include <sstream>

void createFreq::load_xml_file(char* fileName)
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
	TiXmlDocument *pDoc=new TiXmlDocument;
	TiXmlDeclaration *pDeclaration=new TiXmlDeclaration("1.0","UTF-8","");
	pDoc->LinkEndChild(pDeclaration);
    TiXmlElement *confusesets = new TiXmlElement("CONFUSESETS");
	for( TiXmlNode*  item = root->FirstChild( "CONFUSEWORDS" );
			 item;
			 item = item->NextSibling( "CONFUSEWORDS" ) ) 
	{
		TiXmlNode* newNode = new TiXmlElement("CONFUSEWORDS");
		for( TiXmlNode*  cword = item->FirstChild( "CONFUSEWORD" );
			 cword;
			 cword = cword->NextSibling( "CONFUSEWORD" ) )
		{
			TiXmlAttribute* attrib = cword->ToElement()->FirstAttribute();
			string attribval = attrib->Value() ;
			TiXmlNode* name1 = new TiXmlElement("CONFUSEWORD");
			int countSTMT = 0;
			map<string , int>ddict;
			for( TiXmlNode*  fword = cword->FirstChild( "STMT" );
			 fword;
			 fword = fword->NextSibling( "STMT" ) )
			{
				countSTMT++;
				map<string , int> fdict;
				for( TiXmlNode*  tword = fword->FirstChild( "TOKEN" );
				tword;
				tword = tword->NextSibling( "TOKEN" ) )
				{
					const char* val = tword->ToElement()->GetText();
					string tokenText(val);
					if(ddict[tokenText] && !fdict[tokenText])
					{
						ddict[tokenText] += 1;
						fdict[tokenText] = 1;
					}
					else if(!fdict[tokenText])
					{
						ddict[tokenText] = 1;
						fdict[tokenText] = 1;
					}
				}
			}
			name1->ToElement()->SetAttribute("name",attribval.c_str());
			name1->ToElement()->SetAttribute("count" , countSTMT);
			vector<string> wordList; 
			for(map<string , int>::iterator it = ddict.begin(); it != ddict.end(); )
			{
				if(deleteWords[it->first] || it->first == attribval || it->first.size() == 1)
					it++;
				else
				{
					wordList.push_back(it->first);
					it++;
				}
			}
			for(int i = 0 ; i < wordList.size(); i++)
			{
				if(ddict[wordList[i]] > 2)
				{
					TiXmlNode* name2 = new TiXmlElement("FEATUREWORD");
					name2->ToElement()->SetAttribute("name" , wordList[i].c_str());
					stringstream ss;
					string str;
					ss<<ddict[wordList[i]];
					ss>>str;
					name2->InsertEndChild(TiXmlText(str.c_str()));
					name1->InsertEndChild(*name2);
				}
			}
			newNode->InsertEndChild(*name1);
		}
		confusesets->InsertEndChild(*newNode);
	}
	pDoc->InsertEndChild(*confusesets);
	pDoc->SaveFile("FeatureWord.xml");
}

void createFreq::createDel()
{
	deleteWords[","] = 1;
	deleteWords["("] = 1;
	deleteWords[")"] = 1;
	deleteWords[";"] = 1;
	deleteWords["["] = 1;
	deleteWords["]"] = 1;
	deleteWords["{"] = 1;
	deleteWords["}"] = 1;
	deleteWords["!"] = 1;
	deleteWords["."] = 1;
	deleteWords["\""] = 1;
	deleteWords[":"] = 1;
	deleteWords["¡ª"] = 1;
	deleteWords["¡®"] = 1;
	deleteWords["¡¯"] = 1;
	deleteWords["'"] = 1;
	deleteWords["?"] = 1;
	deleteWords["-"] = 1;
	deleteWords["I"] = 1;
	deleteWords["you"] = 1;
	deleteWords["to"] = 1;
	deleteWords["up"] = 1;
	deleteWords["a"] = 1;
	deleteWords["the"] = 1;
	deleteWords["and"] = 1;
	deleteWords["of"] = 1;
	deleteWords["be"] = 1;
	deleteWords["on"] = 1;
	deleteWords["what"] = 1;
	deleteWords["who"] = 1;
	deleteWords["how"] = 1;
	deleteWords["or"] = 1;
	deleteWords["so"] = 1;
	deleteWords["he"] = 1;
	deleteWords["she"] = 1;
	deleteWords["his"] = 1;
	deleteWords["her"] = 1;
	deleteWords["in"] = 1;
}

void createFreq::create()
{
	createDel();
	load_xml_file("stmtAfterTag.xml");
}
createFreq::createFreq()
{
}
createFreq::~createFreq()
{
}