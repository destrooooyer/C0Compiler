#include "asm_generator_kai.h"
#include "util.h"
#include <map>
#include <iostream>
#include <fstream>

using namespace std;

AsmGenerator::AsmGenerator(Table table, vector<Quadruple>imCodes)
{
	paraCount = 0;
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

	asmCodes.push_back("$formatD  byte \"%d\", 0");
	asmCodes.push_back("$formatC  byte \"%c\", 0");
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
	if (imCodes[codeIter].arg1 != "main")
		asmCodes.push_back("@" + imCodes[codeIter].arg1 + " proc");
	else
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
	if (imCodes[codeIter].arg1 != "main")
		asmCodes.push_back("@" + imCodes[codeIter].arg1 + " endp");
	else
		asmCodes.push_back(imCodes[codeIter].arg1 + " endp");
}

string AsmGenerator::getAddrRam(string funcName, string name, vector<string>regOccupied)
{
	string result = "dword ptr ";

	//get the RAM address of an array element
	if (util::isArr(name))
	{
		string arrName = util::getArrName(name);
		string arrIndex = util::getArrIndex(name);

		//get the offset from the index
		string offset;
		if (util::isnumber(arrIndex[0]))
			offset = arrIndex;
		else
			offset = getAddrReg(funcName, arrIndex, regOccupied);

		int loc = addrDescriptor.getRamAddr(funcName, arrName);

		//loc is 0 means that it is a global var
		if (loc == 0)
			result += "[$" + arrName + "+" + offset + "*4]";
		else	//local var
		{
			result += "[ebp";
			if (loc >= 0)
				result += "+";
			result += util::int2string(loc) + "+" + offset + "*4]";
		}
	}
	else
	{
		int loc = addrDescriptor.getRamAddr(funcName, name);
		if (loc == 0)	//global var
			result += "[$" + name + "]";
		else	//local var
		{
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
	vector<string> regOccupiedCopy = regOccupied;
	regOccupiedCopy.push_back(regName);

	string funcName = regManager.getRegContent(regName).first;
	string name = regManager.getRegContent(regName).second;

	//	//it seems that only temp vars need to be saved
	//	//others are saved when assigned
	if (name != ""&&util::isnumber(name[0]) == false)
	{
		// save to ram
		string tempInstr = "mov ";
		tempInstr += getAddrRam(funcName, name, regOccupiedCopy);
		tempInstr += ", " + regName;
		asmCodes.push_back(tempInstr);
	}
	//the content of this reg becomes invalid, the same to the corresponding value in addrDescriptor
	regManager.inValidReg(regName, addrDescriptor);
}

string AsmGenerator::getReg(vector<string> regOccupied)
{
	string temp = regManager.getReg(regOccupied);

	//no available regs now, need to clear a reg first
	if (temp == "*")
	{
		map<string, bool> regAvailability;
		regAvailability["eax"] = true;
		regAvailability["ebx"] = true;
		regAvailability["ecx"] = true;
		regAvailability["edx"] = true;
		regAvailability["esi"] = true;
		regAvailability["edi"] = true;
		for (int i = 0; i < regOccupied.size(); i++)
			regAvailability[regOccupied[i]] = false;

		//only regs not in regOccupied could be chosen
		for (map<string, bool>::iterator iter = regAvailability.begin(); iter != regAvailability.end(); iter++)
		{
			string tempName = regManager.getRegContent(iter->first).second;
			string tempFuncName = regManager.getRegContent(iter->first).first;
			if (iter->second == true)
			{
				genSave(iter->first, regOccupied);
				temp = iter->first;
				break;
			}
		}
	}
	return temp;
}


string AsmGenerator::getAddrReg(string funcName, string name, vector<string> regOccupied)
{
	string temp;

	//save a number to reg and return this reg
	if (util::isnumber(name[0]))
	{
		temp = getReg(regOccupied);

		string tempInstr = "mov ";
		tempInstr += temp + ", ";
		tempInstr += name;
		asmCodes.push_back(tempInstr);

		//this reg is being used
		regManager.load(temp, funcName, name, addrDescriptor);
		return temp;
	}
	else if (util::isArr(name))	//it is an array element
	{
		string arrName = util::getArrName(name);
		string arrIndex = util::getArrIndex(name);

		temp = getReg(regOccupied);

		string tempInstr = "mov ";
		tempInstr += temp + ", ";
		tempInstr += getAddrRam(funcName, name, regOccupied);
		asmCodes.push_back(tempInstr);

		//this reg is being used
		regManager.inValidReg(temp, addrDescriptor);
		regManager.load(temp, funcName, name, addrDescriptor);
		return temp;

	}
	else	//a var
	{
		if (addrDescriptor.isInReg(funcName, name))	//if it is now in reg, return this reg
		{
			temp = addrDescriptor.getRegAddr(funcName, name);
			return temp;
		}
		else	//not in reg, in RAM
		{
			temp = getReg(regOccupied);

			string tempInstr = "mov ";
			tempInstr += temp + ", ";
			tempInstr += getAddrRam(funcName, name, regOccupied);
			asmCodes.push_back(tempInstr);

			regManager.load(temp, funcName, name, addrDescriptor);
			return temp;
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

	//value of arg1 changed, if it's in reg, it becomes invalid
	regManager.inValidSym(funcName, imCodes[loc].arg1, addrDescriptor);
}

void AsmGenerator::genPrintf(string funcName, int loc)
{
	vector<string>regOccupied;
	string arg = getAddrReg(funcName, imCodes[loc].arg1, regOccupied);
	string tempReg;

	asmCodes.push_back("push " + arg);
	tempReg = getReg(regOccupied);

	if (util::isnumber(imCodes[loc].arg1[0]))
	{
		if (imCodes[loc].arg2 == "c")
			asmCodes.push_back("lea " + tempReg + ", $formatCNewLine");
		else
			asmCodes.push_back("lea " + tempReg + ", $formatDNewLine");
	}
	else
	{
		//check it is char or int
		string name;
		//get id
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

	genSave("eax", regOccupied);
	regOccupied.push_back("eax");
	genSave("ebx", regOccupied);
	regOccupied.push_back("ebx");
	genSave("ecx", regOccupied);

	asmCodes.push_back("call printf");
	asmCodes.push_back("add esp, 8");

}

void AsmGenerator::genScanf(string funcName, int loc)
{
	//string arg = getAddrReg(funcName, imCodes[loc].arg1, vector<string>());
	string tempReg;
	string dest;
	vector<string> regOccupied;

	tempReg = getReg(regOccupied);
	dest = getAddrRam(funcName, imCodes[loc].arg1, vector<string>());
	asmCodes.push_back("lea " + tempReg + ", " + dest);
	asmCodes.push_back("push " + tempReg);

	tempReg = getReg(regOccupied);

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


	asmCodes.push_back("push " + tempReg);

	genSave("eax", regOccupied);
	regOccupied.push_back("eax");
	genSave("ebx", regOccupied);
	regOccupied.push_back("ebx");
	genSave("ecx", regOccupied);

	asmCodes.push_back("call scanf");
	asmCodes.push_back("add esp, 8");

}

void AsmGenerator::genPrintStr(int loc)
{
	vector<string> regOccupied;
	string tempReg = getReg(vector<string>());
	asmCodes.push_back("lea " + tempReg + ", " + imCodes[loc].arg1);
	asmCodes.push_back("push " + tempReg);

	genSave("eax", regOccupied);
	regOccupied.push_back("eax");
	genSave("ebx", regOccupied);
	regOccupied.push_back("ebx");
	genSave("ecx", regOccupied);

	asmCodes.push_back("call printf");
	asmCodes.push_back("add esp, 4");

}


void AsmGenerator::genPushPara(string funcName, int loc)
{
	// 	paraCount = 0;
	// 	vector<Quadruple> pushes;
	// 	int locBackUp = loc;
	// 	while (imCodes[loc].op == "pushpara")
	// 		pushes.push_back(imCodes[loc++]);
	// 	paraCount = pushes.size();
	// 	for (int i = pushes.size() - 1; i >= 0; i--)
	// 	{
	// 		string temp = pushes[i].arg1;
	// 		if (util::isnumber(temp[0]))
	// 			asmCodes.push_back("push " + temp);
	// 		else if (util::isArr(temp))
	// 			asmCodes.push_back("push " + getAddrRam(funcName, temp, vector<string>()));
	// 		else
	// 		{
	// 			if (addrDescriptor.isInReg(funcName, temp))
	// 				asmCodes.push_back("push " + addrDescriptor.getRegAddr(funcName, temp));
	// 			else
	// 				asmCodes.push_back("push " + getAddrRam(funcName, temp, vector<string>()));
	// 		}
	// 	}
	paraCount += 1;
	string temp = imCodes[loc].arg1;
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

void AsmGenerator::genCallVoid(int loc)
{
	string funcName = imCodes[loc].arg1;
	vector<string> regOccupied;
	genSave("eax", regOccupied);
	regOccupied.push_back("eax");
	genSave("ebx", regOccupied);
	regOccupied.push_back("ebx");
	genSave("ecx", regOccupied);

	asmCodes.push_back("call @" + funcName);
	asmCodes.push_back("add esp, " + util::int2string(paraCount * 4));
	paraCount = 0;
}

void AsmGenerator::genCall(string funcName, int loc)
{
	vector<string> regOccupied;
	genSave("eax", regOccupied);
	regOccupied.push_back("eax");
	genSave("ebx", regOccupied);
	regOccupied.push_back("ebx");
	genSave("ecx", regOccupied);

	asmCodes.push_back("call @" + imCodes[loc].arg1);
	asmCodes.push_back("add esp, " + util::int2string(paraCount * 4));

	regManager.load("eax", funcName, imCodes[loc].arg2, addrDescriptor);
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
				int result;
				if (imCodes[loc].op == "+")
					result = util::parseInt(imCodes[loc].arg1) + util::parseInt(imCodes[loc].arg2);
				if (imCodes[loc].op == "-")
					result = util::parseInt(imCodes[loc].arg1) - util::parseInt(imCodes[loc].arg2);
				if (imCodes[loc].op == "*")
					result = util::parseInt(imCodes[loc].arg1) * util::parseInt(imCodes[loc].arg2);
				string arg = getAddrReg(funcName, imCodes[loc].arg3, regOccupied);
				asmCodes.push_back("mov " + arg + ", " + util::int2string(result));
			}
			else
			{
				if (!util::isnumber(imCodes[loc].arg1[0]))	//+ a 2 b
				{
					int flag = 0;
					//arg1 is already in reg, use directly
					if (addrDescriptor.isInReg(funcName, imCodes[loc].arg1))
					{
						regArg1 = addrDescriptor.getRegAddr(funcName, imCodes[loc].arg1);
						genSave(regArg1, regOccupied);	//the value in regArg1 is gonna change, so save it first
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

					if (imCodes[loc].op == "+")
						asmCodes.push_back("add " + arg1 + ", " + arg2);
					else if (imCodes[loc].op == "-")
						asmCodes.push_back("sub " + arg1 + ", " + arg2);
					else if (imCodes[loc].op == "*")
						asmCodes.push_back("imul " + arg1 + ", " + arg2);
				}
				else if (!util::isnumber(imCodes[loc].arg2[0]))	//+ 2 a b
				{
					int flag = 0;
					//arg2 is already in reg, use directly
					if (addrDescriptor.isInReg(funcName, imCodes[loc].arg2))
					{
						regArg1 = addrDescriptor.getRegAddr(funcName, imCodes[loc].arg2);
						genSave(regArg1, regOccupied);	//the value in regArg1 is gonna change, so save it first
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

					if (imCodes[loc].op == "+")
						asmCodes.push_back("add " + arg1 + ", " + arg2);
					else if (imCodes[loc].op == "-")
					{
						asmCodes.push_back("neg " + arg1);
						asmCodes.push_back("add " + arg1 + ", " + arg2);
					}
					else if (imCodes[loc].op == "*")
						asmCodes.push_back("imul " + arg1 + ", " + arg2);
				}

				//a+b->c: a+=b, then change the id saved in reg (the id is a) to c
				//chang the id saved in reg
				regManager.inValidReg(arg1, addrDescriptor);
				//if arg3 is in a reg before, it becomes invalid
				regManager.inValidSym(funcName, imCodes[loc].arg3, addrDescriptor);
				regManager.load(arg1, funcName, imCodes[loc].arg3, addrDescriptor);

			}

			if (util::isArr(imCodes[loc].arg3) || imCodes[loc].arg3[0] != '@')
				genSave(arg1, vector<string>());
		}
	}
	else if (imCodes[loc].op == "/")
	{
		vector<string> regOccupied;
		//clear edx and eax
		if (!regManager.isAvailable("eax"))
			genSave("eax", regOccupied);
		regOccupied.push_back("eax");
		if (!regManager.isAvailable("edx"))
			genSave("edx", regOccupied);
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
		asmCodes.push_back("cdq");

		string arg;
		arg = getAddrReg(funcName, imCodes[loc].arg2, regOccupied);
		asmCodes.push_back("idiv " + arg);
		regManager.inValidReg("edx", addrDescriptor);

		//result -> arg3
		//if arg3 is in a reg before, it becomes invalid
		regManager.inValidSym(funcName, imCodes[loc].arg3, addrDescriptor);
		regManager.load("eax", funcName, imCodes[loc].arg3, addrDescriptor);

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
	regManager.upDate(addrDescriptor);
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


pair<string, string> RegManager::getRegContent(string regName)
{
	if (regContent.count(regName))
		return regContent[regName];
}

string RegManager::getReg(vector<string>regOccupied)
{
	for (map<string, bool>::iterator iter = regAvailability.begin(); iter != regAvailability.end(); iter++)
	{
		if (iter->second == true)
		{
			int flag = 0;
			for (int i = 0; i < regOccupied.size(); i++)
				if (regOccupied[i] == iter->first)
				{
					flag = 1;
					break;
				}
			if (flag == 0)
				return iter->first;
		}
	}
	return "*";
}


void RegManager::upDate(AddrDescriptor &ad)
{
	for (map<string, pair<string, string>>::iterator i = regContent.begin(); i != regContent.end(); i++)
	{
		if (i->second.second != "")
		{
			//a number or a array element is saved in this reg
			if (util::isArr(i->second.second) ||
				util::isnumber(i->second.second[0]))
			{
				//invalid this reg
				inValidReg(i->first, ad);
			}
		}
	}
}

void RegManager::load(string regName, string funcName, string symName, AddrDescriptor &ad)
{
	inValidReg(regName, ad);
	regAvailability[regName] = false;
	regContent[regName] = pair<string, string>(funcName, symName);
	ad.setRegAddr(funcName, symName, regName);
}

void RegManager::inValidReg(string regName, AddrDescriptor &ad)
{
	ad.inValid(regContent[regName].first, regContent[regName].second);
	regAvailability[regName] = true;
	regContent[regName] = pair<string, string>("", "");
}

void RegManager::inValidSym(std::string funcName, std::string symName, AddrDescriptor &ad)
{
	ad.inValid(funcName, symName);
	for (map<string, pair<string, string>>::iterator i = regContent.begin(); i != regContent.end(); i++)
	{
		if (i->second.first == funcName&&
			i->second.second == symName)
		{
			regAvailability[i->first] = true;
			regContent[i->first] = pair<string, string>("", "");
		}
	}
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


bool AddrDescriptor::isInReg(string funcName, string name)
{
	string temp = funcName;
	//name is not in table[funcName] maybe it is in table[globle]
	if (table.check(temp, name) == false)
		temp = "globle";

	//if name exists ( it is not a array element or a number
	if (addrReg.count(temp) && addrReg[temp].count(name))
	{
		if (addrReg[temp][name] == "*")
			return false;
		else
			return true;
	}
	return false;
}

string AddrDescriptor::getRegAddr(string funcName, string name)
{
	string temp = funcName;
	//name is not in table[funcName] maybe it is in table[globle]
	if (table.check(temp, name) == false)
		temp = "globle";

	//if name exists ( it is not a array element or a number
	if (addrReg.count(temp) && addrReg[temp].count(name))
	{
		return addrReg[temp][name];
	}
}

int AddrDescriptor::getRamAddr(string funcName, string name)
{
	string temp = funcName;
	//name is not in table[funcName] maybe it is in table[globle]
	if (table.check(temp, name) == false)
		temp = "globle";

	//if name exists ( it is not a array element or a number
	if (addrReg.count(temp) && addrReg[temp].count(name))
	{
		return addrRam[temp][name];
	}
}

void AddrDescriptor::setRegAddr(string funcName, string name, string regName)
{
	string temp = funcName;
	//name is not in table[funcName] maybe it is in table[globle]
	if (table.check(temp, name) == false)
		temp = "globle";

	//if name exists ( it is not a array element or a number
	if (addrReg.count(temp) && addrReg[temp].count(name))
	{
		addrReg[temp][name] = regName;
	}
}

void AddrDescriptor::inValid(string funcName, string name)
{
	string temp = funcName;
	//name is not in table[funcName] maybe it is in table[globle]
	if (table.check(temp, name) == false)
		temp = "globle";

	//if name exists ( it is not a array element or a number
	if (addrReg.count(temp) && addrReg[temp].count(name))
	{
		addrReg[temp][name] = "*";
	}

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