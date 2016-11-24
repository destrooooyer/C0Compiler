#ifndef ASM_GENERATOR_H
#define ASM_GENERATOR_H

#include <string>
#include <vector>

#include "table.h"
#include "intermediate_code_generator.h"


class AsmGenerator
{
public:
	AsmGenerator(Table table, std::vector<Quadruple> imCodes);
	void gen();
	void genData();
	void genCode();
	void printAsmCodes();
	void genFunc(int locBegin);
	void genFuncPrologue(std::string funcName);
	void genFuncEpilogue(std::string funcName);


private:
	Table table;
	std::vector<Quadruple> imCodes;
	std::vector<std::string> asmCodes;

};

class regDescriptor
{

};

class addrDescriptor
{
public:
	std::string getAddr(std::string var)
	{

	}
private:
};


#endif