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
	void genRet(std::string procName);
	void gen4(std::string op, std::string arg1, std::string arg2, std::string arg3);
	void genJz(std::string arg, std::string label);
	void genJmp(std::string label);
	void genLabel(std::string label);
	std::vector<Quadruple> getQuadruples();

private:
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
