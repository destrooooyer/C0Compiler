#ifndef ASM_GENERATOR_KAI_H
#define ASM_GENERATOR_KAI_H

#include <string>
#include <vector>
#include <map>

#include "table.h"
#include "intermediate_code_generator.h"

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
	void reset();
	void inValid(std::string funcName, std::string name);
private:
	std::map<std::string, std::map<std::string, std::string>> addrReg;
	std::map<std::string, std::map<std::string, int>> addrRam;
	Table table;
};

class RegManager
{
public:
	RegManager();
	bool isAvailable(std::string);
	std::pair<std::string, std::string> getRegContent(std::string);
	std::string getReg(std::vector<std::string> regOccupied);
	void load(std::string regName, std::string funcName, std::string symName, AddrDescriptor &ad);
	void reset();
	void inValidReg(std::string regName, AddrDescriptor &ad);
	void inValidSym(std::string funcName, std::string symName, AddrDescriptor &ad);
	void upDate(AddrDescriptor &ad);

private:
	std::map<std::string, bool> regAvailability;
	std::map<std::string, std::pair < std::string, std::string > > regContent;
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
	int paraCount;
	Table table;
	RegManager regManager;
	AddrDescriptor addrDescriptor;
	std::vector<Quadruple> imCodes;
	std::vector<std::string> asmCodes;

	std::string getReg(std::vector<std::string> regOccupied);
	std::string getAddrReg(std::string funcName, std::string name, std::vector<std::string> regOccupied);
	std::string getAddrRam(std::string funcName, std::string name, std::vector<std::string>regOccupied);
	std::map<std::string, std::string> strs;

	void genSave(std::string regName, std::vector<std::string> regOccupied);	//rag->ram

	void genLabel(std::string funcName, int loc);
	void genJmp(int loc);
	void genAssign(std::string funcName, int loc);
	void genPrintf(std::string funcName, int loc);
	void genPrintStr(int loc);

	void genCondition(std::string funcName, int loc);
	void genPushPara(std::string funcName, int loc);
	void genCallVoid(int loc);
	void genCall(std::string funcName, int loc);
	void genReturn(std::string funcName, int loc);
	void genScanf(std::string funcName, int loc);

	void getStrings();
	void genHeader();

};


#endif