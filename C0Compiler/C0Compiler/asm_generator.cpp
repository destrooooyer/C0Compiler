#include "asm_generator.h"
#include "util.h"
#include <map>
#include <iostream>

using namespace std;

AsmGenerator::AsmGenerator(Table table, vector<Quadruple>imCodes)
{
	this->table = table;
	this->imCodes = imCodes;
	this->addrDescriptor = AddrDescriptor(table);
	addrDescriptor.printAddrRam();
}

AsmGenerator::AsmGenerator()
{

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
			else if (i->second.getKind() != "parameter")
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



RegManager::RegManager()
{
	regAvailability["eax"] = true;
	regAvailability["ebx"] = true;
	regAvailability["ecx"] = true;
	regAvailability["edx"] = true;
	regAvailability["esi"] = true;
	regAvailability["edi"] = true;

	regContent["eax"] = pair<string, string>("", "");
	regContent["ebx"] = pair<string, string>("", "");
	regContent["ecx"] = pair<string, string>("", "");
	regContent["edx"] = pair<string, string>("", "");
	regContent["esi"] = pair<string, string>("", "");
	regContent["edi"] = pair<string, string>("", "");
}

bool RegManager::isAvailable(string regName)
{
	if (regAvailability.count(regName))
		return regAvailability[regName];
}

pair<string, string> RegManager::getRegContent(string regName)
{
	if (regContent.count(regName))
		return regContent[regName];
}

string RegManager::getReg()
{
	return "";
}

void RegManager::save(string regName)
{

}

void RegManager::load(string regName, string funcName, string symName)
{

}


AddrDescriptor::AddrDescriptor()
{
}

AddrDescriptor::AddrDescriptor(Table table)
{
	vector<string> tableKeys = table.getKeys();
	for (int i = 0; i < tableKeys.size(); i++)
	{
		int loc = 4;
		map<string, TableItem> tempTable = table.getTable(tableKeys[i]);
		for (map<string, TableItem>::iterator iter = tempTable.begin(); iter != tempTable.end(); iter++)
		{
			if (iter->second.getKind() != "return_function"&&iter->second.getKind() != "void_function"
				&&iter->second.getKind() != "main_function"
				&&iter->second.getKind() != "parameter")
			{
				addrReg[tableKeys[i]][iter->second.getName()] = "*";
				if (tableKeys[i] == "globle")
					addrRam[tableKeys[i]][iter->second.getName()] = iter->second.getName();
				else
				{
					addrRam[tableKeys[i]][iter->second.getName()] = "[ebp-";
					addrRam[tableKeys[i]][iter->second.getName()] += util::int2string(loc) + "]";


					if (iter->second.getKind() == "array")
						loc += iter->second.getValue() * 4;
					else
						loc += 4;
				}
			}
		}
	}
}

bool AddrDescriptor::isInReg(string funcName, string name)
{
	if (addrReg[funcName][name] == "*")
		return false;
	else
		return true;
}

string AddrDescriptor::getRegAddr(string funcName, string name)
{
	return addrReg[funcName][name];
}

string AddrDescriptor::getRamAddr(string funcName, string name)
{
	return addrRam[funcName][name];
}

void AddrDescriptor::setRegAddr(string funcName, string name, string regName)
{
	addrReg[funcName][name] = regName;
}

void AddrDescriptor::printAddrRam()
{
	for (map<string, map<string, string> >::iterator iter = addrRam.begin(); iter != addrRam.end(); iter++)
	{
		cout << "///////////////////////////////////////////\n";
		cout << iter->first << endl;
		for (map<string, string>::iterator iter2 = iter->second.begin(); iter2 != iter->second.end(); iter2++)
		{
			cout << iter2->first << " " << iter2->second << endl;
		}
	}
}