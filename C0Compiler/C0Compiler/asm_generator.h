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
public:
	bool isAvailable(std::string);
	std::pair<std::string, std::string> getRegContent(std::string);

private:
	std::map<std::string, bool> regAvailability;
	std::map<std::string, std::pair < std::string, std::string > > regContent;
};

class addrDescriptor
{
public:
	std::string getAddr(std::string var);
	bool isInReg(std::string funcName, std::string name);
	std::string getReg(std::string funcName, std::string name);
private:
};


#endif