#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include "intermediate_code_generator.h"
#include "table.h"
#include <string>
#include <vector>
#include <map>


class Block
{
public:
	std::vector<Quadruple> codes;
	Block();
	Block(int front, int rear, std::vector<Quadruple> imCodes);
};

class DagNode
{
public:
	std::string value;
	std::string op;
	std::vector<int> fathers;
	int lChild, rChild;
};

class Dag
{
public:
	std::vector<DagNode> nodes;
	std::map < std::string, int> referList;	//<identifier,index of the node>
	void pushCode(Quadruple code);
	std::vector<Quadruple> getCodes();
	void clear();
};

class CSEliminator	//Common Subexpression eliminator
{
public:
	CSEliminator();
	CSEliminator(std::vector<Block> blocks);
	std::vector<Block> getBlocks();
	void execute();

private:
	std::vector<Block> blocks;
	void executeBlock(int i);
};

class Optimizer
{
public:
	Optimizer(std::vector<Quadruple> imCodes, Table table);
	void optimize();
	std::vector<Quadruple> getCodes();
	void printBlocks();
	Table getTable();
private:
	std::vector<Quadruple> imCodes;
	Table table;
	void eliminateConsts();
	void toBlocks();
	void upDateTable();
	void toCodes();
	std::vector<Block> blocks;
};



#endif
