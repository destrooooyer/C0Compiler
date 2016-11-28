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

	asmCodes.push_back("@@formatD  byte \"%d\", 0");
	for (map<string, TableItem>::iterator iter = globleTable.begin(); iter != globleTable.end(); iter++)
	{
		if (iter->second.getKind() == "const" ||
			iter->second.getKind() == "variable")
		{
			string temp;
			temp += "@@";
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
		if (util::isNumber(arrIndex[0]))
		{
			index = arrIndex;
		}
		else
		{
			index = getAddrReg(funcName, arrIndex, regOccupied);
		}

		int loc = addrDescriptor.getRamAddr(funcName, arrName);
		if (tempFunc == "globle")
			result += "[@@" + arrName + "+" + index + "*4]";
		else
		{
			result += "[ebp";
			if (loc >= 0)
				result += "+";
			result += util::int2string(loc) + "-" + index + "*4]";
		}
	}
	else
	{
		if (!table.check(tempFunc, name))
			tempFunc = "globle";

		int loc = addrDescriptor.getRamAddr(funcName, name);
		if (tempFunc == "globle")
			result += "[@@" + name + "]";
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
	regManager.save(regName);
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
	if (util::isNumber(name[0]))
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
		regManager.load(temp, funcName, name);
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

	if (util::isNumber(IMarg1[0]))
		regManager.save(arg1);

	if (util::isNumber(IMarg2[0]))
		regManager.save(arg2);
	/*
		if (util::isNumber(arg1[0]) && util::isNumber(arg2[0]))
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
	arg1 = getAddrReg(funcName, imCodes[loc].arg1, regOccupied);
	regOccupied.push_back(arg1);
	if (util::isNumber(imCodes[loc].arg2[0]))
		arg2 = imCodes[loc].arg2;
	else
		arg2 = getAddrReg(funcName, imCodes[loc].arg2, regOccupied);

	asmCodes.push_back("mov " + arg1 + ", " + arg2);
}

void AsmGenerator::genPrintf(string funcName, int loc)
{
	string arg = getAddrReg(funcName, imCodes[loc].arg1, vector<string>());
	string tempReg;

	asmCodes.push_back("push " + arg);
	tempReg = regManager.getReg();
	if (tempReg == "*")
		tempReg = prepareReg(vector<string>());
	asmCodes.push_back("lea " + tempReg + ", @@formatD");
	asmCodes.push_back("push " + tempReg);
	asmCodes.push_back("call printf");
	asmCodes.push_back("add esp, 8");

}

void AsmGenerator::genPushPara(string funcName, int loc)
{
	vector<Quadruple> pushes;
	int locBackUp = loc;
	while (imCodes[loc].op == "pushpara")
		pushes.push_back(imCodes[loc++]);
	for (int i = pushes.size() - 1; i >= 0; i--)
	{
		string temp = pushes[i].arg1;
		if (util::isNumber(temp[0]))
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
			if (util::isNumber(imCodes[loc].arg2[0]))
				arg2 = imCodes[loc].arg2;
			else
				arg2 = getAddrReg(funcName, imCodes[loc].arg2, regOccupied);

			if (imCodes[loc].op == "+")
				asmCodes.push_back("add " + arg1 + ", " + arg2);
			else if (imCodes[loc].op == "-")
				asmCodes.push_back("sub " + arg1 + ", " + arg2);
			else if (imCodes[loc].op == "*")
				asmCodes.push_back("imul " + arg1 + ", " + arg2);

			if (util::isArr(imCodes[loc].arg3))
				genSave(arg1, vector<string>());
		}
		else if (imCodes[loc].arg2 == imCodes[loc].arg3)
		{
			vector<string> regOccupied;
			if (util::isNumber(imCodes[loc].arg1[0]))
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


			if (util::isArr(imCodes[loc].arg3))
				genSave(arg2, vector<string>());
		}
		else
		{
			vector<string> regOccupied;
			string regArg1 = "";
			if (util::isNumber(imCodes[loc].arg1[0]) && util::isNumber(imCodes[loc].arg2[0]))
			{
				int sum = util::parseInt(imCodes[loc].arg1) + util::parseInt(imCodes[loc].arg2);
				string arg = getAddrReg(funcName, imCodes[loc].arg3, regOccupied);
				asmCodes.push_back("mov " + arg + ", " + util::int2string(sum));
			}
			else
			{
				if (!util::isNumber(imCodes[loc].arg1[0]))	//+ a 2 b
				{
					if (util::isArr(imCodes[loc].arg1) == false && addrDescriptor.isInReg(funcName, imCodes[loc].arg1))
					{
						regArg1 = addrDescriptor.getRegAddr(funcName, imCodes[loc].arg1);
						genSave(regArg1, regOccupied);
					}
					if (regArg1 == "")
						arg1 = getAddrReg(funcName, imCodes[loc].arg1, regOccupied);
					else
						arg1 = regArg1;
					regOccupied.push_back(arg1);
					if (util::isNumber(imCodes[loc].arg2[0]))
						arg2 = imCodes[loc].arg2;
					arg2 = getAddrReg(funcName, imCodes[loc].arg2, regOccupied);
				}
				else if (!util::isNumber(imCodes[loc].arg2[0]))	//+ 2 a b
				{
					if (util::isArr(imCodes[loc].arg2) == false && addrDescriptor.isInReg(funcName, imCodes[loc].arg2))
					{
						regArg1 = addrDescriptor.getRegAddr(funcName, imCodes[loc].arg2);
						genSave(regArg1, regOccupied);
					}
					if (regArg1 == "")
						arg1 = getAddrReg(funcName, imCodes[loc].arg2, regOccupied);
					else
						arg1 = regArg1;
					regOccupied.push_back(arg1);
					if (util::isNumber(imCodes[loc].arg1[0]))
						arg2 = imCodes[loc].arg1;
					arg2 = getAddrReg(funcName, imCodes[loc].arg1, regOccupied);
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

			if (util::isArr(imCodes[loc].arg3))
				genSave(arg1, vector<string>());
		}
	}
	else if (imCodes[loc].op == "/")
	{
		vector<string> regOccupied;
		//prepare edx and eax
		if (!regManager.isAvailable("eax"))
		{
			regManager.save("eax");
			genSave("eax", vector<string>());
		}
		if (!regManager.isAvailable("edx"))
		{
			regManager.save("edx");
			genSave("edx", vector<string>());
		}
		//0 -> edx
		asmCodes.push_back("and, edx, 0");
		regManager.load("edx", funcName, "*");
		//arg1 -> eax
		string tempInstr = "mov ";
		tempInstr += "eax, ";
		if (util::isNumber(imCodes[loc].arg1[0]))
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
		regManager.load("eax", funcName, "*");

		//idiv arg2
		regOccupied.push_back("eax");
		regOccupied.push_back("edx");
		string arg;
		if (util::isNumber(imCodes[loc].arg2[0]))
			arg = imCodes[loc].arg2;
		else
			arg = getAddrReg(funcName, imCodes[loc].arg2, regOccupied);
		asmCodes.push_back("idiv " + arg);
		regManager.save("edx");

		//result -> arg3
		regManager.load("eax", funcName, imCodes[loc].arg3);
		if (addrDescriptor.isInReg(funcName, imCodes[loc].arg3))
			regManager.save(addrDescriptor.getRegAddr(funcName, imCodes[loc].arg3));
		addrDescriptor.setRegAddr(funcName, imCodes[loc].arg3, "eax");
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
	asmCodes.push_back("pop ebx");
	asmCodes.push_back("pop edi");
	asmCodes.push_back("pop esi");
	asmCodes.push_back("mov esp, ebp");
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
					addrRam[tableKeys[i]][iter->second.getName()] = (-loc);

					if (iter->second.getKind() == "array")
						loc += iter->second.getValue() * 4;
					else
						loc += 4;
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