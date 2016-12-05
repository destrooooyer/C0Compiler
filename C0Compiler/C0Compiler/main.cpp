#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <iostream>
#include <vector>

#include "global.h"

using namespace std;

vector<Symbol> LexicalAnalyse(string file)
{
	vector<Symbol>symbles;
	LexAnalyzer lex(file);
	//lex.infoSwitch = true;
	lex.infoSwitch = false;
	if (lex.isSourceOpen() == false)
	{
		cout << "���ļ�ʧ��\n";
		return symbles;
	}
	Symbol temp = lex.getNextSymble();
	while (temp.getType() != "EOF")
	{
		symbles.push_back(temp);
		temp = lex.getNextSymble();
	}
	return symbles;

}

void printIntermediateCodes(vector<Quadruple> intermediateCodes)
{
	ofstream fout("imc.txt");
	for (int i = 0; i < intermediateCodes.size(); i++)
	{
		cout << intermediateCodes[i].op << "\t" << intermediateCodes[i].arg1 << "\t" << intermediateCodes[i].arg2 << "\t" << intermediateCodes[i].arg3 << endl;
		fout << intermediateCodes[i].op << "\t" << intermediateCodes[i].arg1 << "\t" << intermediateCodes[i].arg2 << "\t" << intermediateCodes[i].arg3 << endl;
	}
}


int main()
{
	cout << "����Դ�ļ����ļ���:\t";
	//cin >> sourceFile;
	//sourceFile = "D:/documents/ѧУ���/����/�﷨����/14061120_������_�﷨����/14061120_test.txt";
	//sourceFile = "D:\\documents\\ѧУ���\\����\\��������\\test.txt";
	sourceFile = "C:\\Users\\DESTR\\Desktop\\14061120_������_���Գ���һ\\test.c";
	cout << sourceFile;

	cout << endl;
	vector<Symbol> symbles = LexicalAnalyse(sourceFile);
	Table table;
	vector<Quadruple> intermediateCodes;
	SyntaxAnalyzer syntax(symbles);

	//syntax.infoSwitch = true;
	syntax.infoSwitch = false;
	syntax.program();
	syntax.printErrors();
	system("pause");

	table = syntax.getTable();
	table.print();

	intermediateCodes = syntax.getIntermediateCodes();

	Optimizer optimizer(intermediateCodes,table);
	optimizer.optimize();
	intermediateCodes = optimizer.getCodes();
	printIntermediateCodes(intermediateCodes);

	AsmGenerator asmGenerator(table, intermediateCodes);
	asmGenerator.gen();
	asmGenerator.printAsmCodes();

END:;
	system("pause");
}