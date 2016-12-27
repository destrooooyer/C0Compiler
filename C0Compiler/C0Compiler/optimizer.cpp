#include "optimizer.h"
#include "util.h"
#include<iostream>
using namespace std;
Optimizer::Optimizer(vector<Quadruple> imCodes, Table table)
{
	this->imCodes = imCodes;
	this->table = table;
}

void Optimizer::optimize()
{
	//eliminateConsts();
	toBlocks();
	printBlocks();
}

vector<Quadruple> Optimizer::getCodes()
{
	return imCodes;
}

void Optimizer::eliminateConsts()
{
	vector<Quadruple> tempCodes = imCodes;
	string funcName;
	for (int i = 0; i < tempCodes.size(); i++)
	{
		if (tempCodes[i].op == "proc")
		{
			funcName = tempCodes[i].arg1;
		}
		else if (tempCodes[i].op != "proc"&&
			tempCodes[i].op != "endp"&&
			tempCodes[i].op != "call"&&
			tempCodes[i].op != "callvoid"&&
			tempCodes[i].op != "printf")
		{
			string temp = tempCodes[i].arg1;
			if (table.check(funcName, temp) && table.get(funcName, temp).getKind() == "const")
				tempCodes[i].arg1 = util::int2string(table.get(funcName, temp).getValue());
			else if (table.check("globle", temp) && table.get("globle", temp).getKind() == "const")
				tempCodes[i].arg1 = util::int2string(table.get("globle", temp).getValue());

			temp = tempCodes[i].arg2;
			if (table.check(funcName, temp) && table.get(funcName, temp).getKind() == "const")
				tempCodes[i].arg2 = util::int2string(table.get(funcName, temp).getValue());
			else if (table.check("globle", temp) && table.get("globle", temp).getKind() == "const")
				tempCodes[i].arg2 = util::int2string(table.get("globle", temp).getValue());

			temp = tempCodes[i].arg3;
			if (table.check(funcName, temp) && table.get(funcName, temp).getKind() == "const")
				tempCodes[i].arg3 = util::int2string(table.get(funcName, temp).getValue());
			else if (table.check("globle", temp) && table.get("globle", temp).getKind() == "const")
				tempCodes[i].arg3 = util::int2string(table.get("globle", temp).getValue());
		}
		else if (tempCodes[i].op == "printf")
		{
			string temp = tempCodes[i].arg1;
			TableItem item;
			if (table.check(funcName, temp) && table.get(funcName, temp).getKind() == "const")
			{
				item = table.get(funcName, temp);
				tempCodes[i].arg1 = util::int2string(item.getValue());
			}
			else if (table.check("globle", temp) && table.get("globle", temp).getKind() == "const")
			{
				item = table.get("globle", temp);
				tempCodes[i].arg1 = util::int2string(item.getValue());
			}
			if (item.getType() == "char")
				tempCodes[i].arg2 = "c";
		}
	}
	imCodes = tempCodes;

}


void Optimizer::toBlocks()
{
	int blockFront = 0;
	int blockRear = 1;
	while (blockRear < imCodes.size())
	{
		if (imCodes[blockRear].op == "proc" ||
			imCodes[blockRear].op == "label"
			)
		{
			if (blockFront < blockRear)
				blocks.push_back(Block(blockFront, blockRear, imCodes));
			blockFront = blockRear;
		}
		else if (imCodes[blockRear].op == "jmp" ||
			imCodes[blockRear].op == "jz" ||
			imCodes[blockRear].op == "jnz" ||
			imCodes[blockRear].op == "return")
		{
			if (blockFront < blockRear + 1)
				blocks.push_back(Block(blockFront, blockRear + 1, imCodes));
			blockFront = blockRear + 1;
		}
		blockRear++;
	}
}

void Optimizer::printBlocks()
{
	for (int i = 0; i < blocks.size(); i++)
	{
		cout << "--------------------------------------\n";
		for (int j = 0; j < blocks[i].codes.size(); j++)
		{
			cout << blocks[i].codes[j].op << "\t" <<
				blocks[i].codes[j].arg1 << "\t" <<
				blocks[i].codes[j].arg2 << "\t" <<
				blocks[i].codes[j].arg3 << endl;
		}
	}
}

Block::Block(int front, int rear, vector<Quadruple> imCodes)
{
	for (int i = front; i < rear; i++)
		codes.push_back(imCodes[i]);
}