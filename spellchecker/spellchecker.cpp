
/*
	* spellchecker
	* this class is to entry the system and call all functions 
	* created by lijunbao 
	* copyright USTC
	* 22.05.2014
*/
#include "stdafx.h"
#include "tinyxml.h"
#include <iostream>
#include "testWinnow.h"
#include <string>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <afxinet.h>
#include <stdio.h>
#include "stdafx.h"
#include "iostream"
#include "winnow.h"
#include "createStmtTag.h"
#include "createMultiWords.h"
#include "createFreq.h"
#include "Lexer.h"
#include "creatValue.h"
#include "loadDirect.h"
using namespace std;
int main()
{
	//createMultiWords test;
	//test.createMulti();
	//createStmtTag test;
	//test.create();
	//createFreq testF;
	//testF.create();

	//creatValue testC;
	//testC.load("FeatureWord.xml");
	//testC.test();
	//testC.calculate();
	//Winnow testW;
	//testW.winnow();
	testWinnow testT;
	testT.test(1.1);
	//loadDirect load;
	//load.createDirect();
}
	