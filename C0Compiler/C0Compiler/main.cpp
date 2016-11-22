#include <iostream>
#include <stdlib.h>
#include <iostream>
#include <vector>

#include "syntax_analyzer.h"
#include "global.h"
#include "lexical_analyzer.h"
#include "table.h"

using namespace std;

vector<Symble> LexicalAnalyse(string file)
{
	vector<Symble>symbles;
	LexAnalyzer lex(file);
	lex.infoSwitch = true;
	if (lex.isSourceOpen() == false)
	{
		cout << "���ļ�ʧ��\n";
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


int main()
{
	cout << "����Դ�ļ����ļ���:\t";
	//cin >> sourceFile;
	sourceFile = "D:/documents/ѧУ���/����/�﷨����/14061120_������_�﷨����/14061120_test.txt";
	cout << sourceFile;
	vector<Symble> symbles = LexicalAnalyse(sourceFile);
	Table table;
	SyntaxAnalyzer syntax(symbles,table);
	syntax.infoSwitch = true;
	syntax.program();
	syntax.printErrors();
	table = syntax.getTable();
	table.print();

END:;
	system("pause");
}