#include "asm_generator.h"
#include "util.h"
#include <map>
#include <iostream>
#include <fstream>

using namespace std;

AsmGenerator::AsmGenerator(Table table, vector<Quadruple>imCodes)
{
	this->table = table;
	this->imCodes = imCodes;
	this->addrDescriptor = AddrDescriptor(table);
	addrDescriptor.printAddrRam();
	getStrings();
}

AsmGenerator::AsmGenerator()
{

}

void AsmGenerator::getStrings()
{
	int s = 0;
	for (int i = 0; i < imCodes.size(); i++)
	{
		if (imCodes[i].op == "printStr")
		{
			strs.insert(pair<string, string>("$str" + util::int2string(s), imCodes[i].arg1));
			imCodes[i].arg1 = "$str" + util::int2string(s);
			s++;
		}
	}
}

void AsmGenerator::genHeader()
{
	asmCodes.push_back(".386");
	asmCodes.push_back(".model flat, stdcall  ");
	asmCodes.push_back("option casemap: none");
	asmCodes.push_back("includelib msvcrt.lib");
	asmCodes.push_back("printf proto c");
	asmCodes.push_back("scanf proto c");
}

void AsmGenerator::genData()
{
	asmCodes.push_back("");
	asmCodes.push_back("");
	asmCodes.push_back(".data");
	map<string, TableItem> globleTable = table.getTable("globle");

	asmCodes.push_back("$formatD  byte \"%d \", 0");
	asmCodes.push_back("$formatC  byte \"%c \", 0");
	asmCodes.push_back("$formatDNewLine  byte \"%d\", 0AH, 0");
	asmCodes.push_back("$formatCNewLine  byte \"%c\", 0AH, 0");
	for (map<string, TableItem>::iterator iter = globleTable.begin(); iter != globleTable.end(); iter++)
	{
		if (iter->second.getKind() == "const" ||
			iter->second.getKind() == "variable")
		{
			string temp;
			temp += "$";
			temp += iter->second.getName();
			temp += " dword ";
			temp += util::int2string(iter->second.getValue());
			asmCodes.push_back(temp);
		}
		else if (iter->second.getKind() == "array")
		{
			string temp;
			temp += "$";
			temp += iter->second.getName();
			temp += " dword ";
			temp += util::int2string(iter->second.getValue());
			temp += " dup(0)";
			asmCodes.push_back(temp);
		}
	}
	for (map<string, string>::iterator iter = strs.begin(); iter != strs.end(); iter++)
	{
		asmCodes.push_back(iter->first + " byte " + iter->second + ", 0AH, 0");
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
	addrDescriptor.reset();
	regManager.reset();

	int codeIter = locBegin;
	asmCodes.push_back("");
	asmCodes.push_back("; " + imCodes[codeIter].arg1);
	//gen proc
	asmCodes.push_back(imCodes[codeIter].arg1 + " proc");

	genFuncPrologue(imCodes[locBegin].arg1);
	codeIter++;
	asmCodes.push_back("");
	asmCodes.push_back("; " + imCodes[locBegin].arg1 + " body");

	while (imCodes[codeIter].op != "endp"&&codeIter < imCodes.size())
	{
		genStatement(codeIter, imCodes[locBegin].arg1);
		codeIter++;
	}

	genFuncEpilogue(imCodes[locBegin].arg1);

	//gen endp
	asmCodes.push_back(imCodes[codeIter].arg1 + " endp");
}

string AsmGenerator::getAddrRam(string funcName, string name, vector<string>regOccupied)
{
	string result = "dword ptr ";
	string tempFunc = funcName;
	if (util::isArr(name))
	{
		string arrName = util::getArrName(name);
		string arrIndex = util::getArrIndex(name);
		if (!table.check(tempFunc, arrName))
			tempFunc = "globle";

		string index;
		if (util::isnumber(arrIndex[0]))
		{
			index = arrIndex;
		}
		else
		{
			index = getAddrReg(funcName, arrIndex, regOccupied);
		}

		int loc = addrDescriptor.getRamAddr(funcName, arrName);
		if (tempFunc == "globle")
			result += "[$" + arrName + "+" + index + "*4]";
		else
		{
			result += "[ebp";
			if (loc >= 0)
				result += "+";
			result += util::int2string(loc) + "+" + index + "*4]";
		}
	}
	else
	{
		if (!table.check(tempFunc, name))
			tempFunc = "globle";

		int loc = addrDescriptor.getRamAddr(funcName, name);
		if (tempFunc == "globle")
			result += "[$" + name + "]";
		else {
			result += "[ebp";
			if (loc >= 0)
				result += "+";
			result += util::int2string(loc) + "]";
		}
	}

	return result;
}

void AsmGenerator::genSave(string regName, vector<string> regOccupied)
{
	string funcName = regManager.getRegContent(regName).first;
	string name = regManager.getRegContent(regName).second;

	if (util::isArr(name))
	{
		string arrName = util::getArrName(name);
		string arrIndex = util::getArrIndex(name);

		// save to ram
		string tempInstr = "mov ";
		tempInstr += getAddrRam(funcName, name, regOccupied);
		tempInstr += ", " + regName;
		asmCodes.push_back(tempInstr);
	}
	else
	{
		// save to ram
		string tempInstr = "mov ";
		tempInstr += getAddrRam(funcName, name, regOccupied);
		tempInstr += ", " + regName;
		asmCodes.push_back(tempInstr);

		//this symble is not in reg any more
		addrDescriptor.setRegAddr(funcName, name, "*");
	}
	regManager.save(regName);
}

void AsmGenerator::genLoad(std::string funcName, std::string name, vector<string> regOccupied)
{

}


//when the regs are all being used, prepare an empty reg
string AsmGenerator::prepareReg(vector<string>regOccupied)
{
	string result;
	map<string, bool> regAvailability;
	regAvailability["eax"] = true;
	regAvailability["ebx"] = true;
	regAvailability["ecx"] = true;
	regAvailability["edx"] = true;
	regAvailability["esi"] = true;
	regAvailability["edi"] = true;
	for (int i = 0; i < regOccupied.size(); i++)
		regAvailability[regOccupied[i]] = false;
	for (map<string, bool>::iterator iter = regAvailability.begin(); iter != regAvailability.end(); iter++)
	{
		string tempName = regManager.getRegContent(iter->first).second;
		string tempFuncName = regManager.getRegContent(iter->first).first;
		if (iter->second == true)
		{
			regOccupied.push_back(iter->first);
			genSave(iter->first, regOccupied);
			result = iter->first;
			break;
		}
	}

	return result;
}


string AsmGenerator::getAddrReg(string funcName, string name, vector<string> regOccupied)
{
	string temp;
	if (util::isnumber(name[0]))
	{
		temp = regManager.getReg();
		if (temp == "*")		//no empty regs
		{
			temp = prepareReg(regOccupied);
		}
		string tempInstr = "mov ";
		tempInstr += temp + ", ";
		tempInstr += name;
		asmCodes.push_back(tempInstr);
		//regManager.load(temp, funcName, name);
		return temp;
	}
	else
	{
		if (util::isArr(name))	//sym is an array element
		{
			string arrName = util::getArrName(name);
			string arrIndex = util::getArrIndex(name);


			//if (table.check(funcName, arrName) == false)
			//	funcName = "globle";

			temp = regManager.getReg();
			if (temp == "*")		//no empty regs
			{
				temp = prepareReg(regOccupied);
			}

			regManager.load(temp, funcName, name);
			regOccupied.push_back(temp);
			string tempInstr = "mov ";
			tempInstr += temp + ", ";
			tempInstr += getAddrRam(funcName, name, regOccupied);
			asmCodes.push_back(tempInstr);
			return temp;

		}
		else
		{
			//if (table.check(funcName, name) == false)
			//	funcName = "globle";

			if (addrDescriptor.isInReg(funcName, name))	//sym is in reg
			{
				temp = addrDescriptor.getRegAddr(funcName, name);
				return temp;
			}
			else	//sym is in ram, mov
			{
				int locTemp = addrDescriptor.getRamAddr(funcName, name);
				temp = regManager.getReg();
				if (temp == "*")		//no empty reg
				{
					temp = prepareReg(regOccupied);
				}
				regManager.load(temp, funcName, name);
				addrDescriptor.setRegAddr(funcName, name, temp);
				string tempInstr = "mov ";
				tempInstr += temp + ", ";
				tempInstr += getAddrRam(funcName, name, regOccupied);
				asmCodes.push_back(tempInstr);
				return temp;
			}
		}
	}
}

void AsmGenerator::genCondition(string funcName, int loc)
{
	int flag = 0;
	string op = imCodes[loc].op;
	string IMarg1 = imCodes[loc].arg1;
	string IMarg2 = imCodes[loc].arg2;
	vector<string> regOccupied;
	string arg1 = getAddrReg(funcName, IMarg1, regOccupied);
	regOccupied.push_back(arg1);
	string arg2 = getAddrReg(funcName, IMarg2, regOccupied);

	asmCodes.push_back("cmp " + arg1 + ", " + arg2);
	if (imCodes[loc + 1].op == "jz") flag = -1;
	else if (imCodes[loc + 1].op == "jnz") flag = 1;

	string label;
	for (int i = 0; i < imCodes[loc + 1].arg1.length() - 1; i++)
		label += imCodes[loc + 1].arg1[i];

	if (op == ">")
	{
		if (flag == 1)
			asmCodes.push_back("jg " + label);
		else if (flag == -1)
			asmCodes.push_back("jle " + label);
	}
	else if (op == ">=")
	{
		if (flag == 1)
			asmCodes.push_back("jge " + label);
		else if (flag == -1)
			asmCodes.push_back("jl " + label);
	}
	else if (op == "<")
	{
		if (flag == 1)
			asmCodes.push_back("jl " + label);
		else if (flag == -1)
			asmCodes.push_back("jge " + label);
	}
	else if (op == "<=")
	{
		if (flag == 1)
			asmCodes.push_back("jle " + label);
		else if (flag == -1)
			asmCodes.push_back("jg " + label);
	}
	else if (op == "==")
	{
		if (flag == 1)
			asmCodes.push_back("je " + label);
		else if (flag == -1)
			asmCodes.push_back("jne " + label);
	}
	else if (op == "!=")
	{
		if (flag == 1)
			asmCodes.push_back("jne " + label);
		else if (flag == -1)
			asmCodes.push_back("je " + label);
	}

	if (util::isnumber(IMarg1[0]))
		regManager.save(arg1);

	if (util::isnumber(IMarg2[0]))
		regManager.save(arg2);
	/*
		if (util::isnumber(arg1[0]) && util::isnumber(arg2[0]))
		{
			bool result = 0;
			if (op == ">")
				result = (util::parseInt(arg1) > util::parseInt(arg2));
			else if (op == ">=")
				result = (util::parseInt(arg1) >= util::parseInt(arg2));
			else if (op == "<")
				result = (util::parseInt(arg1) < util::parseInt(arg2));
			else if (op == "<=")
				result = (util::parseInt(arg1) <= util::parseInt(arg2));
			else if (op == "==")
				result = (util::parseInt(arg1) == util::parseInt(arg2));
			else if (op == "!=")
				result = (util::parseInt(arg1) != util::parseInt(arg2));

		}
	*/
}

void AsmGenerator::genLabel(std::string funcName, int loc)
{
	addrDescriptor.reset();
	regManager.reset();
	asmCodes.push_back(imCodes[loc].arg1);
}

void AsmGenerator::genJmp(int loc)
{
	string temp = "jmp ", temp2;
	temp += imCodes[loc].arg1;
	for (int i = 0; i < temp.length() - 1; i++)
		temp2 += temp[i];
	asmCodes.push_back(temp2);
}

void AsmGenerator::genAssign(string funcName, int loc)
{
	string arg1, arg2;
	vector<string> regOccupied;
	arg1 = getAddrRam(funcName, imCodes[loc].arg1, regOccupied);
	if (util::isnumber(imCodes[loc].arg2[0]))
		arg2 = imCodes[loc].arg2;
	else
		arg2 = getAddrReg(funcName, imCodes[loc].arg2, regOccupied);

	asmCodes.push_back("mov " + arg1 + ", " + arg2);
	if (addrDescriptor.isInReg(funcName, imCodes[loc].arg1))
	{
		string regTemp = addrDescriptor.getRegAddr(funcName, imCodes[loc].arg1);
		regManager.save(regTemp);
		addrDescriptor.setRegAddr(funcName, imCodes[loc].arg1, "*");
	}
	else
	{
		regManager.clearSym(funcName, imCodes[loc].arg1);
	}

	// 	if (util::isArr(imCodes[loc].arg1))
	// 		genSave(arg1, vector<string>());
}

void AsmGenerator::clearEACDX()
{
	regManager.save("eax");
	regManager.save("ecx");
	regManager.save("edx");
	addrDescriptor.clearEACDX();
}

void AsmGenerator::genPrintf(string funcName, int loc)
{
	string arg = getAddrReg(funcName, imCodes[loc].arg1, vector<string>());
	string tempReg;

	asmCodes.push_back("push " + arg);
	tempReg = regManager.getReg();
	if (tempReg == "*")
		tempReg = prepareReg(vector<string>());

	if (util::isnumber(imCodes[loc].arg1[0]))
	{
		asmCodes.push_back("lea " + tempReg + ", $formatD");
	}
	else
	{
		string name;
		if (util::isArr(imCodes[loc].arg1))
			name = util::getArrName(imCodes[loc].arg1);
		else
			name = imCodes[loc].arg1;
		if (!table.check(funcName, name))
			funcName = "globle";
		if (table.get(funcName, name).getType() == "char")
			asmCodes.push_back("lea " + tempReg + ", $formatCNewLine");
		else
			asmCodes.push_back("lea " + tempReg + ", $formatDNewLine");
	}

	asmCodes.push_back("push " + tempReg);

	if (!regManager.isAvailable("eax"))
		genSave("eax", vector<string>());
	if (!regManager.isAvailable("ecx"))
		genSave("ecx", vector<string>());
	if (!regManager.isAvailable("edx"))
		genSave("edx", vector<string>());

	clearEACDX();

	asmCodes.push_back("call printf");
	asmCodes.push_back("add esp, 8");

}

void AsmGenerator::genScanf(string funcName, int loc)
{
	//string arg = getAddrReg(funcName, imCodes[loc].arg1, vector<string>());
	string tempReg;
	string dest;


	tempReg = regManager.getReg();
	if (tempReg == "*")
		tempReg = prepareReg(vector<string>());
	dest = getAddrRam(funcName, imCodes[loc].arg1, vector<string>());
	asmCodes.push_back("lea " + tempReg + ", " + dest);

	asmCodes.push_back("push " + tempReg);
	tempReg = regManager.getReg();
	if (tempReg == "*")
		tempReg = prepareReg(vector<string>());


	string name;
	if (util::isArr(imCodes[loc].arg1))
		name = util::getArrName(imCodes[loc].arg1);
	else
		name = imCodes[loc].arg1;
	if (!table.check(funcName, name))
		funcName = "globle";
	if (table.get(funcName, name).getType() == "char")
		asmCodes.push_back("lea " + tempReg + ", [$formatC]");
	else
		asmCodes.push_back("lea " + tempReg + ", [$formatD]");


	//asmCodes.push_back("lea " + tempReg + ", [$formatD]");
	asmCodes.push_back("push " + tempReg);

	if (!regManager.isAvailable("eax"))
		genSave("eax", vector<string>());
	if (!regManager.isAvailable("ecx"))
		genSave("ecx", vector<string>());
	if (!regManager.isAvailable("edx"))
		genSave("edx", vector<string>());

	clearEACDX();

	asmCodes.push_back("call scanf");
	asmCodes.push_back("add esp, 8");

}

void AsmGenerator::genPrintStr(int loc)
{
	string tempReg = regManager.getReg();
	if (tempReg == "*")
		tempReg = prepareReg(vector<string>());
	asmCodes.push_back("lea " + tempReg + ", " + imCodes[loc].arg1);
	asmCodes.push_back("push " + tempReg);

	if (!regManager.isAvailable("eax"))
		genSave("eax", vector<string>());
	if (!regManager.isAvailable("ecx"))
		genSave("ecx", vector<string>());
	if (!regManager.isAvailable("edx"))
		genSave("edx", vector<string>());

	clearEACDX();

	asmCodes.push_back("call printf");
	asmCodes.push_back("add esp, 4");

}


void AsmGenerator::genPushPara(string funcName, int loc)
{
	paraCount = 0;
	vector<Quadruple> pushes;
	int locBackUp = loc;
	while (imCodes[loc].op == "pushpara")
		pushes.push_back(imCodes[loc++]);
	paraCount = pushes.size();
	for (int i = pushes.size() - 1; i >= 0; i--)
	{
		string temp = pushes[i].arg1;
		if (util::isnumber(temp[0]))
			asmCodes.push_back("push " + temp);
		else if (util::isArr(temp))
			asmCodes.push_back("push " + getAddrRam(funcName, temp, vector<string>()));
		else
		{
			if (addrDescriptor.isInReg(funcName, temp))
				asmCodes.push_back("push " + addrDescriptor.getRegAddr(funcName, temp));
			else
				asmCodes.push_back("push " + getAddrRam(funcName, temp, vector<string>()));
		}
	}
}

void AsmGenerator::genCallVoid(int loc)
{
	string funcName = imCodes[loc].arg1;
	if (!regManager.isAvailable("eax"))
		genSave("eax", vector<string>());
	if (!regManager.isAvailable("ecx"))
		genSave("ecx", vector<string>());
	if (!regManager.isAvailable("edx"))
		genSave("edx", vector<string>());

	clearEACDX();

	asmCodes.push_back("call " + funcName);
	asmCodes.push_back("add esp, " + util::int2string(paraCount * 4));
	paraCount = 0;
}

void AsmGenerator::genCall(string funcName, int loc)
{
	if (!regManager.isAvailable("eax"))
		genSave("eax", vector<string>());
	if (!regManager.isAvailable("ecx"))
		genSave("ecx", vector<string>());
	if (!regManager.isAvailable("edx"))
		genSave("edx", vector<string>());

	clearEACDX();

	asmCodes.push_back("call " + imCodes[loc].arg1);
	asmCodes.push_back("add esp, " + util::int2string(paraCount * 4));
	regManager.load("eax", funcName, imCodes[loc].arg2);
	addrDescriptor.setRegAddr(funcName, imCodes[loc].arg2, "eax");
	paraCount = 0;
}

void AsmGenerator::genReturn(string funcName, int loc)
{
	string arg;
	vector<string> regOccupied;
	if (imCodes[loc].arg1 != "")
	{
		if (util::isnumber(imCodes[loc].arg1[0]))
			arg = imCodes[loc].arg1;
		else
			arg = getAddrReg(funcName, imCodes[loc].arg1, regOccupied);
		if (arg != "eax")
			asmCodes.push_back("mov eax, " + arg);
	}
	asmCodes.push_back("jmp @" + funcName + "_epilogue");

}


void AsmGenerator::genStatement(int loc, string funcName)
{
	string arg1, arg2;
	string instruction;
	string regTemp;
	if (imCodes[loc].op == "+" || imCodes[loc].op == "-" || imCodes[loc].op == "*")
	{
		if (imCodes[loc].arg1 == imCodes[loc].arg3)
		{
			vector<string> regOccupied;
			arg1 = getAddrReg(funcName, imCodes[loc].arg1, regOccupied);
			regOccupied.push_back(arg1);
			if (util::isnumber(imCodes[loc].arg2[0]))
				arg2 = imCodes[loc].arg2;
			else
				arg2 = getAddrReg(funcName, imCodes[loc].arg2, regOccupied);

			if (imCodes[loc].op == "+")
				asmCodes.push_back("add " + arg1 + ", " + arg2);
			else if (imCodes[loc].op == "-")
				asmCodes.push_back("sub " + arg1 + ", " + arg2);
			else if (imCodes[loc].op == "*")
				asmCodes.push_back("imul " + arg1 + ", " + arg2);

			if (util::isArr(imCodes[loc].arg3) || imCodes[loc].arg3[0] != '@')
				genSave(arg1, vector<string>());
		}
		else if (imCodes[loc].arg2 == imCodes[loc].arg3)
		{
			vector<string> regOccupied;
			if (util::isnumber(imCodes[loc].arg1[0]))
				arg1 = imCodes[loc].arg1;
			else
				arg1 = getAddrReg(funcName, imCodes[loc].arg1, regOccupied);
			regOccupied.push_back(arg1);
			arg2 = getAddrReg(funcName, imCodes[loc].arg2, regOccupied);

			if (imCodes[loc].op == "+")
				asmCodes.push_back("add " + arg2 + ", " + arg1);
			else if (imCodes[loc].op == "-")
			{
				asmCodes.push_back("neg " + arg2);
				asmCodes.push_back("add " + arg2 + ", " + arg1);
			}
			else if (imCodes[loc].op == "*")
				asmCodes.push_back("imul " + arg2 + ", " + arg1);


			if (util::isArr(imCodes[loc].arg3) || imCodes[loc].arg3[0] != '@')
				genSave(arg1, vector<string>());
		}
		else
		{
			vector<string> regOccupied;
			string regArg1 = "";
			if (util::isnumber(imCodes[loc].arg1[0]) && util::isnumber(imCodes[loc].arg2[0]))
			{
				int sum;
				if (imCodes[loc].op == "+")
					sum = util::parseInt(imCodes[loc].arg1) + util::parseInt(imCodes[loc].arg2);
				if (imCodes[loc].op == "-")
					sum = util::parseInt(imCodes[loc].arg1) - util::parseInt(imCodes[loc].arg2);
				if (imCodes[loc].op == "*")
					sum = util::parseInt(imCodes[loc].arg1) * util::parseInt(imCodes[loc].arg2);
				string arg = getAddrReg(funcName, imCodes[loc].arg3, regOccupied);
				asmCodes.push_back("mov " + arg + ", " + util::int2string(sum));
			}
			else
			{
				if (!util::isnumber(imCodes[loc].arg1[0]))	//+ a 2 b
				{
					int flag = 0;
					if (util::isArr(imCodes[loc].arg1) == false && addrDescriptor.isInReg(funcName, imCodes[loc].arg1))
					{
						regArg1 = addrDescriptor.getRegAddr(funcName, imCodes[loc].arg1);
						flag = 1;
					}
					if (regArg1 == "")
						arg1 = getAddrReg(funcName, imCodes[loc].arg1, regOccupied);
					else
						arg1 = regArg1;
					regOccupied.push_back(arg1);
					if (util::isnumber(imCodes[loc].arg2[0]))
						arg2 = imCodes[loc].arg2;
					else
						arg2 = getAddrReg(funcName, imCodes[loc].arg2, regOccupied);
					regOccupied.push_back(arg2);
					if (flag)
						genSave(regArg1, regOccupied);
				}
				else if (!util::isnumber(imCodes[loc].arg2[0]))	//+ 2 a b
				{
					int flag = 0;
					if (util::isArr(imCodes[loc].arg2) == false && addrDescriptor.isInReg(funcName, imCodes[loc].arg2))
					{
						regArg1 = addrDescriptor.getRegAddr(funcName, imCodes[loc].arg2);
						flag = 1;
					}
					if (regArg1 == "")
						arg1 = getAddrReg(funcName, imCodes[loc].arg2, regOccupied);
					else
						arg1 = regArg1;
					regOccupied.push_back(arg1);
					if (util::isnumber(imCodes[loc].arg1[0]))
						arg2 = imCodes[loc].arg1;
					else
						arg2 = getAddrReg(funcName, imCodes[loc].arg1, regOccupied);
					regOccupied.push_back(arg2);
					if (flag)
						genSave(regArg1, regOccupied);
				}

				if (imCodes[loc].op == "+")
					asmCodes.push_back("add " + arg1 + ", " + arg2);
				else if (imCodes[loc].op == "-")
					asmCodes.push_back("sub " + arg1 + ", " + arg2);
				else if (imCodes[loc].op == "*")
					asmCodes.push_back("imul " + arg1 + ", " + arg2);

				regManager.load(arg1, funcName, imCodes[loc].arg3);
				addrDescriptor.setRegAddr(funcName, imCodes[loc].arg1, "*");
				addrDescriptor.setRegAddr(funcName, imCodes[loc].arg3, arg1);

			}

			if (util::isArr(imCodes[loc].arg3) || imCodes[loc].arg3[0] != '@')
				genSave(arg1, vector<string>());
		}
	}
	else if (imCodes[loc].op == "/")
	{
		vector<string> regOccupied;
		//prepare edx and eax
		if (!regManager.isAvailable("eax"))
		{
			genSave("eax", regOccupied);
		}
		regManager.load("eax", funcName, "*");
		regOccupied.push_back("eax");
		if (!regManager.isAvailable("edx"))
		{
			genSave("edx", regOccupied);
		}
		////0 -> edx
		//asmCodes.push_back("and edx, 0");
		regManager.load("edx", funcName, "*");
		regOccupied.push_back("edx");
		//arg1 -> eax
		string tempInstr = "mov ";
		tempInstr += "eax, ";
		if (util::isnumber(imCodes[loc].arg1[0]))
		{
			tempInstr += imCodes[loc].arg1;
		}
		else
		{
			if (addrDescriptor.isInReg(funcName, imCodes[loc].arg1))
				tempInstr += addrDescriptor.getRegAddr(funcName, imCodes[loc].arg1);
			else
				tempInstr += getAddrRam(funcName, imCodes[loc].arg1, regOccupied);
		}
		asmCodes.push_back(tempInstr);
		//regManager.load("eax", funcName, "*");
		asmCodes.push_back("cdq");
		//idiv arg2
		//regOccupied.push_back("eax");
		//regOccupied.push_back("edx");
		string arg;
		//		if (util::isnumber(imCodes[loc].arg2[0]))
		//			arg = imCodes[loc].arg2;
		//		else
		arg = getAddrReg(funcName, imCodes[loc].arg2, regOccupied);
		asmCodes.push_back("idiv " + arg);
		regManager.save("edx");

		//result -> arg3
		regManager.load("eax", funcName, imCodes[loc].arg3);
		if (addrDescriptor.isInReg(funcName, imCodes[loc].arg3))
			regManager.save(addrDescriptor.getRegAddr(funcName, imCodes[loc].arg3));
		addrDescriptor.setRegAddr(funcName, imCodes[loc].arg3, "eax");


		if (util::isArr(imCodes[loc].arg3) || imCodes[loc].arg3[0] != '@')
			genSave("eax", vector<string>());
	}
	else if (imCodes[loc].op == "label")
	{
		genLabel(funcName, loc);
	}
	else if (imCodes[loc].op == "jmp")
	{
		genJmp(loc);
	}
	else if (imCodes[loc].op == "=")
	{
		genAssign(funcName, loc);
	}
	else if (imCodes[loc].op == "printf")
	{
		genPrintf(funcName, loc);
	}
	else if (imCodes[loc].op == "scanf")
	{
		genScanf(funcName, loc);
	}
	else if (imCodes[loc].op == "printStr")
	{
		genPrintStr(loc);
	}
	else if (imCodes[loc].op == ">" ||
		imCodes[loc].op == ">=" ||
		imCodes[loc].op == "<" ||
		imCodes[loc].op == "<=" ||
		imCodes[loc].op == "==" ||
		imCodes[loc].op == "!=")
	{
		genCondition(funcName, loc);
	}
	else if (imCodes[loc].op == "pushpara")
	{
		if (imCodes[loc - 1].op != "pushpara")
			genPushPara(funcName, loc);
	}
	else if (imCodes[loc].op == "call")
	{
		genCall(funcName, loc);
	}
	else if (imCodes[loc].op == "callvoid")
	{
		genCallVoid(loc);
	}
	else if (imCodes[loc].op == "return")
	{
		genReturn(funcName, loc);
	}
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
		int size = 4;
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
	asmCodes.push_back("@" + funcName + "_epilogue:");
	asmCodes.push_back("pop ebx");
	asmCodes.push_back("pop edi");
	asmCodes.push_back("pop esi");
	asmCodes.push_back("mov esp, ebp");
	asmCodes.push_back("pop ebp");
	asmCodes.push_back("ret");
}

void AsmGenerator::gen()
{
	genHeader();
	genData();
	genCode();
	asmCodes.push_back("end main");
}



void AsmGenerator::printAsmCodes()
{
	ofstream fout("2.asm");
	for (int i = 0; i < asmCodes.size(); i++)
	{
		fout << asmCodes[i] << endl;
		cout << asmCodes[i] << endl;
	}
	fout.close();
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

void RegManager::reset()
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

void RegManager::clearSym(string funcName, string name)
{
	for (map<string, pair<string, string>>::iterator i = regContent.begin(); i != regContent.end(); i++)
	{
		if (i->second.first == funcName&&
			i->second.second == name)
		{
			save(i->first);
		}
	}
}

pair<string, string> RegManager::getRegContent(string regName)
{
	if (regContent.count(regName))
		return regContent[regName];
}

string RegManager::getReg()
{
	for (map<string, bool>::iterator iter = regAvailability.begin(); iter != regAvailability.end(); iter++)
	{
		if (iter->second == true)
			return iter->first;
	}
	return "*";
}

void RegManager::save(string regName)
{
	regAvailability[regName] = true;
}

void RegManager::load(string regName, string funcName, string symName)
{
	regAvailability[regName] = false;
	regContent[regName] = pair<string, string>(funcName, symName);
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
					addrRam[tableKeys[i]][iter->second.getName()] = 0;
				else
				{

					if (iter->second.getKind() == "array")
					{
						loc += iter->second.getValue() * 4;
						loc -= 4;
						addrRam[tableKeys[i]][iter->second.getName()] = (-loc);
						loc += 4;
					}
					else
					{
						addrRam[tableKeys[i]][iter->second.getName()] = (-loc);
						loc += 4;
					}
				}
			}
			else if (iter->second.getKind() == "parameter")
			{
				addrReg[tableKeys[i]][iter->second.getName()] = "*";
				addrRam[tableKeys[i]][iter->second.getName()] = (iter->second.getValue() * 4 + 4);

			}
		}
	}
	this->table = table;
}

void AddrDescriptor::reset()
{
	for (map<string, map<string, string>>::iterator iter = addrReg.begin(); iter != addrReg.end(); iter++)
	{
		for (map<string, string>::iterator i = iter->second.begin(); i != iter->second.end(); i++)
			i->second = "*";
	}
}

void AddrDescriptor::clearEACDX()
{
	for (map<string, map<string, string>>::iterator iter = addrReg.begin(); iter != addrReg.end(); iter++)
	{
		for (map<string, string>::iterator i = iter->second.begin(); i != iter->second.end(); i++)
		{
			if (i->second == "eax" ||
				i->second == "ecx" ||
				i->second == "edx")
				i->second = "*";
		}
	}
}


bool AddrDescriptor::isInReg(string funcName, string name)
{
	string temp = funcName;
	if (util::isArr(name))
		return false;
	if (table.check(temp, name) == false)
		temp = "globle";
	if (addrReg[temp][name] == "*")
		return false;
	else
		return true;
}

string AddrDescriptor::getRegAddr(string funcName, string name)
{
	string temp = funcName;
	if (util::isArr(name))
		return "";
	if (table.check(temp, name) == false)
		temp = "globle";
	return addrReg[temp][name];
}

int AddrDescriptor::getRamAddr(string funcName, string name)
{
	string temp = funcName;
	if (util::isArr(name))
		return 0;
	if (table.check(temp, name) == false)
		temp = "globle";
	return addrRam[temp][name];
}

void AddrDescriptor::setRegAddr(string funcName, string name, string regName)
{
	string temp = funcName;
	if (util::isArr(name))
		return;
	if (table.check(temp, name) == false)
		temp = "globle";
	if (addrReg.count(temp) && addrReg[temp].count(name))
		addrReg[temp][name] = regName;
}

void AddrDescriptor::printAddrRam()
{
	for (map<string, map<string, int> >::iterator iter = addrRam.begin(); iter != addrRam.end(); iter++)
	{
		cout << "///////////////////////////////////////////\n";
		cout << iter->first << endl;
		for (map<string, int>::iterator iter2 = iter->second.begin(); iter2 != iter->second.end(); iter2++)
		{
			cout << iter2->first << " " << iter2->second << endl;
		}
	}
}