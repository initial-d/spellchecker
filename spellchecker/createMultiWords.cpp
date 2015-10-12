/*
	* createMultiWords
	* this class is for writing words to xml files 
	* created by yangziqiang 
	* copyright USTC
	* 22.05.2014
*/
#include "createMultiWords.h"

createMultiWords::createMultiWords()
{
}
createMultiWords::~createMultiWords()
{
}
void createMultiWords::createMulti()
{
	TiXmlDocument *pDoc=new TiXmlDocument;
	TiXmlDeclaration *pDeclaration=new TiXmlDeclaration("1.0","UTF-8","");
	pDoc->LinkEndChild(pDeclaration);
    TiXmlElement *confusesets = new TiXmlElement("CONFUSESETS");
	TiXmlNode* newNode = new TiXmlElement("CONFUSESET");
	ifstream fin ("words.txt");
	int temp = 0;
	char line[100];
	while(fin.getline(line , 100))
	{
		string tmpline(line);
		if(tmpline.size() == 0)
		{
			newNode->ToElement()->SetAttribute("size",temp);
			confusesets->InsertEndChild(*newNode);
			newNode = new TiXmlElement("CONFUSESET");
			cout<<endl;
			temp = 0;
		}
		else
		{
			cout<<tmpline<<endl;
			TiXmlNode* name4 = new TiXmlElement("WORD");
			name4->InsertEndChild(TiXmlText(tmpline.c_str()));
			newNode->InsertEndChild(*name4);
			temp++;
		}
	}
	pDoc->InsertEndChild(*confusesets);
	pDoc->SaveFile("multiWords.xml.xml");
}