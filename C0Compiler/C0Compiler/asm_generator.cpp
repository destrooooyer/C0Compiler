#include "asm_generator.h"
#include "util.h"
#include <map>
#include <iostream>

using namespace std;

AsmGenerator::AsmGenerator(Table table, vector<Quadruple>imCodes)
{
	this->table = table;
	this->imCodes = imCodes;
}

void AsmGenerator::genData()
{
	asmCodes.push_back("");
	asmCodes.push_back("");
	asmCodes.push_back(".data");
	map<string, TableItem> globleTable = table.getTable("globle");

	for (map<string, TableItem>::iterator iter = globleTable.begin(); iter != globleTable.end(); iter++)
	{
		if (iter->second.getKind() == "const" ||
			iter->second.getKind() == "variable")
		{
			string temp;
			temp += iter->second.getName();
			temp += " dword ";
			temp += util::int2string(iter->second.getValue());
			asmCodes.push_back(temp);
		}
	}
}

void AsmGenerator::genCode()
{
	asmCodes.push_back("");
	asmCodes.push_back("");
	asmCodes.push_back(".code");

	for (int i = 0; i < imCodes.size(); i++)
	{
		if (imCodes[i].op == "proc")
		{
			genFunc(i);
		}
	}
}

void AsmGenerator::genFunc(int locBegin)
{
	int codeIter = locBegin;
	asmCodes.push_back("");
	asmCodes.push_back("; " + imCodes[codeIter].arg1);
	//gen proc
	asmCodes.push_back(imCodes[codeIter].arg1 + " proc");

	genFuncPrologue(imCodes[codeIter].arg1);

	while (imCodes[codeIter].op != "endp"&&codeIter < imCodes.size())
	{
		codeIter++;
	}

	genFuncEpilogue(imCodes[locBegin].arg1);

	//gen endp
	asmCodes.push_back(imCodes[codeIter].arg1 + " endp");
}

void AsmGenerator::genFuncPrologue(string funcName)
{
	asmCodes.push_back("");
	asmCodes.push_back("; " + funcName + " prologue");
	asmCodes.push_back("push ebp");
	asmCodes.push_back("mov ebp, esp");
	map<string, TableItem> funcTable = table.getTable(funcName);
	if (funcTable.size() != 0)
	{
		int size = 0;
		for (map<string, TableItem>::iterator i = funcTable.begin(); i != funcTable.end(); i++)
		{
			if (i->second.getKind() == "array")
				size += i->second.getValue() * 4;
			else
				size += 4;
		}
		asmCodes.push_back("sub esp, " + util::int2string(size));
	}
	asmCodes.push_back("push esi");
	asmCodes.push_back("push edi");
	asmCodes.push_back("push ebx");
}

void AsmGenerator::genFuncEpilogue(string funcName)
{
	asmCodes.push_back("");
	asmCodes.push_back("; " + funcName + " epilogue");
	asmCodes.push_back("pop ebx");
	asmCodes.push_back("pop edi");
	asmCodes.push_back("pop esi");
	asmCodes.push_back("move esp, ebp");
	asmCodes.push_back("pop ebp");
	asmCodes.push_back("ret");
}

void AsmGenerator::gen()
{
	genData();
	genCode();
}



void AsmGenerator::printAsmCodes()
{
	for (int i = 0; i < asmCodes.size(); i++)
		cout << asmCodes[i] << endl;
}