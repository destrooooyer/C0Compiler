#include <iostream>
#include <stdlib.h>
#include <iostream>
#include <vector>

#include "global.h"

using namespace std;

vector<Symble> LexicalAnalyse(string file)
{
	vector<Symble>symbles;
	LexAnalyzer lex(file);
	lex.infoSwitch = true;
	if (lex.isSourceOpen() == false)
	{
		cout << "打开文件失败\n";
		return symbles;
	}
	Symble temp = lex.getNextSymble();
	while (temp.getType() != "EOF")
	{
		symbles.push_back(temp);
		temp = lex.getNextSymble();
	}
	return symbles;

}

void printIntermediateCodes(vector<Quadruple> intermediateCodes)
{
	for (int i = 0; i < intermediateCodes.size(); i++)
		cout << intermediateCodes[i].op << "\t" << intermediateCodes[i].arg1 << "\t" << intermediateCodes[i].arg2 << "\t" << intermediateCodes[i].arg3 << endl;
}


int main()
{
	cout << "输入源文件的文件名:\t";
	//cin >> sourceFile;
	//sourceFile = "D:/documents/学校相关/编译/语法分析/14061120_刘润泽_语法分析/14061120_test.txt";
	sourceFile = "D:\\documents\\学校相关\\编译\\代码生成\\test.txt";
	cout << sourceFile;


	vector<Symble> symbles = LexicalAnalyse(sourceFile);
	Table table;
	vector<Quadruple> intermediateCodes;
	SyntaxAnalyzer syntax(symbles);

	syntax.infoSwitch = true;
	syntax.program();
	syntax.printErrors();

	table = syntax.getTable();
	table.print();

	intermediateCodes = syntax.getIntermediateCodes();
	printIntermediateCodes(intermediateCodes);

END:;
	system("pause");
}