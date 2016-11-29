#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include "intermediate_code_generator.h"
#include "table.h"
#include <string>
#include <vector>


class Optimizer
{
public:
	Optimizer(std::vector<Quadruple> imCodes, Table table);
	void optimize();
	std::vector<Quadruple> getCodes();
private:
	std::vector<Quadruple> imCodes;
	Table table;
	void eliminateConsts();
};


#endif
