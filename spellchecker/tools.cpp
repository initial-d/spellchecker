/*
	* filetools
	* this class is to definit some useful funtions
	* created by yangziqiang 
	* copyright USTC
	* 22.05.2014
*/
#include "tools.h"
#include <stdlib.h>
#include <fstream>
void filetools::load_inputXML_file(char* fileName , map<string , map<string , float>> &valueDict , map<string , vector<string>> &oneToMany)
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
	for( TiXmlNode*  item = root->FirstChild( "CONFUSEWORDS" );
			 item;
			 item = item->NextSibling( "CONFUSEWORDS" ) ) 
	{
		vector<string> many;
		for( TiXmlNode*  cword = item->FirstChild( "CONFUSEWORD" );
			 cword;
			 cword = cword->NextSibling( "CONFUSEWORD" ) )
		{
			map<string , float> fDict;
			TiXmlAttribute* cwordattrib = cword->ToElement()->FirstAttribute();
			//string cname = cwordattrib->Name() ;
			string cval = cwordattrib->Value() ;
			many.insert(many.end() , cval);
			//printf("%s = %s\n" , cname , cval);
			for( TiXmlNode*  fword = cword->FirstChild( "FEATUREWORD" );
			 fword;
			 fword = fword->NextSibling( "FEATUREWORD" ) )
			{
				TiXmlAttribute* attrib = fword->ToElement()->FirstAttribute();
				//string name = attrib->Name() ;
				string attribval = attrib->Value() ;
				const char* val = fword->ToElement()->GetText();
				fDict[attribval] = (float)atof(val);
				valueDict[cval] = fDict;
			}
		}
		
		for(int i = 0 ; i < many.size();i++)
		{
			oneToMany[many[i]] = many;
		}
	}
	
}

void filetools::load_stmtXML_file(char *fileName , map<string , vector<string>> &stmtDict , float percent)
{
	const char* filepath = fileName;
	TiXmlDocument doc(filepath);
	bool loadOkay = doc.LoadFile();
	// faile to load 'phonebookdata.xml'.
	if (!loadOkay) {	
		printf( "Could not load test file %s. Error='%s'. Exiting.\n", filepath,doc.ErrorDesc() );
		exit( 1 );
	}
	ofstream fout ("howMuch.txt");
	TiXmlElement* root = doc.RootElement();
	for( TiXmlNode*  item = root->FirstChild( "CONFUSESET" );
			 item;
			 item = item->NextSibling( "CONFUSESET" ) ) 
	{
		for( TiXmlNode*  cword = item->FirstChild( "WORD" );
			 cword;
			 cword = cword->NextSibling( "WORD" ) )
		{
			vector<string> stmtList , stmts;
			TiXmlAttribute* cwordattrib = cword->ToElement()->FirstAttribute();
			//string cname = cwordattrib->Name() ;
			string cval = cwordattrib->Value() ;
			//printf("%s = %s\n" , cname , cval);
			for( TiXmlNode*  fword = cword->FirstChild( "STMT" );
			 fword;
			 fword = fword->NextSibling( "STMT" ) )
			{
				TiXmlAttribute* attrib = fword->ToElement()->FirstAttribute();
				//string name = attrib->Name() ;
				const char* val = fword->ToElement()->GetText();
				string chtost(val);
				stmts.insert(stmts.end() , chtost);
			}
			if(percent == 0.8)
				for(int i = stmts.size() * (1 - percent) ; i < stmts.size() ; i++)
				{
					stmtList.insert(stmtList.end() , stmts[i]);
				}
			else
				for(int i = 0 ; i < stmts.size() * percent ; i++)
				{
					stmtList.insert(stmtList.end() , stmts[i]);
				}
			stmtDict[cval] = stmtList;
			fout<<cval<<endl;
		}
	}
}

filetools::filetools()
{
}
filetools::~filetools()
{
}