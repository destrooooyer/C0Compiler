#ifndef INTERMEDIATE_CODE_GENERATOR_H
#define INTERMEDIATE_CODE_GENERATOR_H

#include "table.h"
#include <string>
#include <vector>

class Quadruple
{
public:
	Quadruple(std::string op, std::string arg1, std::string arg2, std::string arg3);
	std::string op;
	std::string arg1;
	std::string arg2;
	std::string arg3;
};


class IMCodeGenerator
{
public:
	IMCodeGenerator();

	void genProc(std::string procName);
	void genAssign(std::string src, std::string dst);
	void genEndp(std::string procName);
	void gen4(std::string op, std::string arg1, std::string arg2, std::string arg3);
	void genJz(std::string arg, std::string label);
	void genJnz(std::string arg, std::string label);
	void genJmp(std::string label);
	void genLabel(std::string label);
	void genReturn(std::string arg);
	void genCall(std::string funName,std::string dst);
	void genVoidCall(std::string funName);
	void genPushpara(std::string arg);
	void genScanf(std::string arg);
	void genPrintf(std::string arg);
	void genPrintStr(std::string str);
	std::vector<Quadruple> getQuadruples();

	std::vector<Quadruple> quadruples;
};

class TempNameManager
{
public:
	TempNameManager();
	std::string getTempName();
private:
	int count;
};

class LabelManager
{
public:
	LabelManager();
	std::string getLabel();
private:
	int count;
};

#endif
