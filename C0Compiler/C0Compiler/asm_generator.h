#ifndef ASM_GENERATOR_H
#define ASM_GENERATOR_H

#include <string>
#include <vector>
#include <map>

#include "table.h"
#include "intermediate_code_generator.h"


class RegManager
{
public:
	RegManager();
	bool isAvailable(std::string);
	std::pair<std::string, std::string> getRegContent(std::string);
	std::string getReg();
	void save(std::string regName);
	void load(std::string regName, std::string funcName, std::string symName);

private:
	std::map<std::string, bool> regAvailability;
	std::map<std::string, std::pair < std::string, std::string > > regContent;
};


class AddrDescriptor
{
public:
	AddrDescriptor();
	AddrDescriptor(Table table);
	bool isInReg(std::string funcName, std::string name);
	std::string getRegAddr(std::string funcName, std::string name);
	int getRamAddr(std::string funcName, std::string name);
	void setRegAddr(std::string funcName, std::string name, std::string regName);
	void printAddrRam();
private:
	std::map<std::string, std::map<std::string, std::string>> addrReg;
	std::map<std::string, std::map<std::string, int>> addrRam;
	Table table;
};


class AsmGenerator
{
public:
	AsmGenerator();
	AsmGenerator(Table table, std::vector<Quadruple> imCodes);
	void gen();
	void genData();
	void genCode();
	void printAsmCodes();
	void genFunc(int locBegin);
	void genFuncPrologue(std::string funcName);
	void genFuncEpilogue(std::string funcName);
	void genStatement(int loc, std::string funcName);


private:
	Table table;
	RegManager regManager;
	AddrDescriptor addrDescriptor;
	std::vector<Quadruple> imCodes;
	std::vector<std::string> asmCodes;

	std::string getAddrReg(std::string funcName, std::string name, std::vector<std::string> regOccupied);
	std::string prepareReg(std::vector<std::string>regOccupied);
	std::string getAddrRam(std::string funcName, std::string name, std::vector<std::string>regOccupied);

	void genSave(std::string regName, std::vector<std::string> regOccupied);	//rag->ram
	void genLoad(std::string funcName, std::string name, std::vector<std::string> regOccupied);	//ram->reg

};


#endif