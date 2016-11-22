#include "intermediate_code_generator.h"

using namespace std;

TempNameManager::TempNameManager()
{
	count = 0;
}

string TempNameManager::getTempName()
{
	string temp = "temp";
	count++;
	temp += ('0' + count);
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
	temp += ('0' + count);
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

void IMCodeGenerator::genRet(string procName)
{
	quadruples.push_back(Quadruple("ret", procName, "", ""));
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
	quadruples.push_back(Quadruple("jz", arg, label, ""));
}

void IMCodeGenerator::genJmp(string label)
{
	quadruples.push_back(Quadruple("jmp", label, "", ""));
}

void IMCodeGenerator::genLabel(string label)
{
	quadruples.push_back(Quadruple("label", label, "", ""));
}


vector<Quadruple> IMCodeGenerator::getQuadruples()
{
	return quadruples;
}