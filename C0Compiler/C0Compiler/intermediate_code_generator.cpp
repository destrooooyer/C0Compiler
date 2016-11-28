#include "intermediate_code_generator.h"
#include "util.h"

using namespace std;

TempNameManager::TempNameManager()
{
	count = 0;
}

string TempNameManager::getTempName()
{
	string temp = "@temp";
	count++;
	temp += util::int2string(count);
	return temp;
}

LabelManager::LabelManager()
{
	count = 0;
}

string LabelManager::getLabel()
{
	string temp = "L";
	count++;
	temp += util::int2string(count);
	temp += ':';
	return temp;
}


Quadruple::Quadruple(string op, string arg1, string arg2, string arg3)
{
	this->op = op;
	this->arg1 = arg1;
	this->arg2 = arg2;
	this->arg3 = arg3;
}

IMCodeGenerator::IMCodeGenerator()
{
}

void IMCodeGenerator::genProc(string procName)
{
	quadruples.push_back(Quadruple("proc", procName, "", ""));
}

void IMCodeGenerator::genEndp(string procName)
{
	quadruples.push_back(Quadruple("endp", procName, "", ""));
}

void IMCodeGenerator::gen4(std::string op, string arg1, string arg2, string arg3)
{
	quadruples.push_back(Quadruple(op, arg1, arg2, arg3));
}

void IMCodeGenerator::genAssign(string src, string dst)
{
	quadruples.push_back(Quadruple("=", src, dst, ""));
}

void IMCodeGenerator::genJz(string arg, string label)
{
	quadruples.push_back(Quadruple("jz", label, "", ""));
}

void IMCodeGenerator::genJnz(string arg, string label)
{
	quadruples.push_back(Quadruple("jnz", label, "", ""));
}

void IMCodeGenerator::genJmp(string label)
{
	quadruples.push_back(Quadruple("jmp", label, "", ""));
}

void IMCodeGenerator::genLabel(string label)
{
	quadruples.push_back(Quadruple("label", label, "", ""));
}

void IMCodeGenerator::genReturn(string arg)
{
	quadruples.push_back(Quadruple("return", arg, "", ""));
}

void IMCodeGenerator::genCall(string funcName, string dst)
{
	quadruples.push_back(Quadruple("call", funcName, dst, ""));
}

void IMCodeGenerator::genVoidCall(string funcName)
{
	quadruples.push_back(Quadruple("callvoid", funcName, "", ""));
}

void IMCodeGenerator::genPushpara(string arg)
{
	quadruples.push_back(Quadruple("pushpara", arg, "", ""));
}

void IMCodeGenerator::genScanf(string arg)
{
	quadruples.push_back(Quadruple("scanf", arg, "", ""));
}

void IMCodeGenerator::genPrintf(string arg)
{
	quadruples.push_back(Quadruple("printf", arg, "", ""));
}

void IMCodeGenerator::genPrintStr(string str)
{
	quadruples.push_back(Quadruple("printStr", str, "", ""));
}



vector<Quadruple> IMCodeGenerator::getQuadruples()
{
	return quadruples;
}