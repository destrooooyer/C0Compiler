#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include "intermediate_code_generator.h"
#include "table.h"
#include <string>
#include <vector>


class Block
{
public:
	std::vector<Quadruple> codes;
	Block();
	Block(int front, int rear, std::vector<Quadruple> imCodes);
};

class Optimizer
{
public:
	Optimizer(std::vector<Quadruple> imCodes, Table table);
	void optimize();
	std::vector<Quadruple> getCodes();
	void printBlocks();
private:
	std::vector<Quadruple> imCodes;
	Table table;
	void eliminateConsts();
	void toBlocks();
	std::vector<Block> blocks;
};



#endif
