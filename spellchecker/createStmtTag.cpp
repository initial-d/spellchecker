/*
	* createStmtTag
	* this class is for split scentence to words  
	* created by lijunbaos 
	* copyright USTC
	* 22.05.2014
*/
#include "createStmtTag.h"
#include "Lexer.h"
#include <sstream>

vector<string> createStmtTag::tagStmt(string stmt)
{
	vector<string> temp;
	Lexer lexer(stmt);
	temp = lexer.nextWord();
	return temp;
}

void createStmtTag::load_xml_file(char* fileName)
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
    TiXmlElement *stmts = new TiXmlElement("STMTS");
	for( TiXmlNode*  item = root->FirstChild( "CONFUSESET" );
			 item;
			 item = item->NextSibling( "CONFUSESET" ) ) 
	{
		TiXmlNode* newNode = new TiXmlElement("CONFUSEWORDS");
		for( TiXmlNode*  cword = item->FirstChild( "WORD" );
			 cword;
			 cword = cword->NextSibling( "WORD" ) )
		{
			TiXmlAttribute* attrib = cword->ToElement()->FirstAttribute();
			string attribval = attrib->Value() ;
			TiXmlNode* name1 = new TiXmlElement("CONFUSEWORD");
			name1->ToElement()->SetAttribute("name",attribval.c_str());
			for( TiXmlNode*  fword = cword->FirstChild( "STMT" );
			 fword;
			 fword = fword->NextSibling( "STMT" ) )
			{
				TiXmlNode* name2 = new TiXmlElement("STMT");
				const char* val = fword->ToElement()->GetText();
				name2->ToElement()->SetAttribute("name",val);
				vector<string> analyseList;
				string stmt(val);
				analyseList = tagStmt(stmt);
				for(int i = 0  ; i < analyseList.size() ; i++)
				{
					TiXmlNode* name3 = new TiXmlElement("TOKEN");
					name3->ToElement()->SetAttribute("tag","DD");
					name3->InsertEndChild(TiXmlText(analyseList[i].c_str()));
					name2->InsertEndChild(*name3);
				}
				name1->InsertEndChild(*name2);
			}
			newNode->InsertEndChild(*name1);
		}
		stmts->InsertEndChild(*newNode);
	}
	pDoc->InsertEndChild(*stmts);
	pDoc->SaveFile("stmtAfterTag.xml");
}

void createStmtTag::create()
{
	load_xml_file("stmt.xml");
}

createStmtTag::createStmtTag()
{
}

createStmtTag::~createStmtTag()
{

}