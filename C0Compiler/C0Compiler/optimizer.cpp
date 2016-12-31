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
	constFolding();
	toBlocks();
	printBlocks();
	CSEliminator cse(blocks);
	cse.execute();
	blocks = cse.getBlocks();
	printBlocks();
	toCodes();
	upDateTable();
}

vector<Quadruple> Optimizer::getCodes()
{
	return imCodes;
}

void Optimizer::constFolding()
{
	for (int i = 0; i < imCodes.size(); i++)
	{
		if (imCodes[i].op == "+")
		{
			if (util::isnumber(imCodes[i].arg1[0]) &&
				util::isnumber(imCodes[i].arg2[0]))
			{
				imCodes[i].op = "=";
				imCodes[i].arg2 = util::int2string(util::parseInt(imCodes[i].arg1) + util::parseInt(imCodes[i].arg2));
				imCodes[i].arg1 = imCodes[i].arg3;
			}
		}
		else if (imCodes[i].op == "-")
		{
			if (util::isnumber(imCodes[i].arg1[0]) &&
				util::isnumber(imCodes[i].arg2[0]))
			{
				imCodes[i].op = "=";
				imCodes[i].arg2 = util::int2string(util::parseInt(imCodes[i].arg1) - util::parseInt(imCodes[i].arg2));
				imCodes[i].arg1 = imCodes[i].arg3;
			}
		}
		else if (imCodes[i].op == "*")
		{
			if (util::isnumber(imCodes[i].arg1[0]) &&
				util::isnumber(imCodes[i].arg2[0]))
			{
				imCodes[i].op = "=";
				imCodes[i].arg2 = util::int2string(util::parseInt(imCodes[i].arg1) * util::parseInt(imCodes[i].arg2));
				imCodes[i].arg1 = imCodes[i].arg3;
			}
		}
		else if (imCodes[i].op == "/")
		{
			if (util::isnumber(imCodes[i].arg1[0]) &&
				util::isnumber(imCodes[i].arg2[0]))
			{
				imCodes[i].op = "=";
				imCodes[i].arg2 = util::int2string(util::parseInt(imCodes[i].arg1) / util::parseInt(imCodes[i].arg2));
				imCodes[i].arg1 = imCodes[i].arg3;
			}
		}
	}
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
	if (blockFront < blockRear)
		blocks.push_back(Block(blockFront, blockRear, imCodes));
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

void Optimizer::toCodes()
{
	imCodes.clear();
	for (int i = 0; i < blocks.size(); i++)
	{
		imCodes.insert(imCodes.end(), blocks[i].codes.begin(), blocks[i].codes.end());
	}
}

void Optimizer::upDateTable()
{
	string funcName = "globle";
	for (int i = 0; i < imCodes.size(); i++)
	{
		if (imCodes[i].op == "proc")
			funcName = imCodes[i].arg1;
		else if (imCodes[i].op == "+" ||
			imCodes[i].op == "-" ||
			imCodes[i].op == "*" ||
			imCodes[i].op == "/" ||
			imCodes[i].op == "=")
		{
			if (imCodes[i].arg1[0] == '@'&&imCodes[i].arg1[1] == 'T')
				table.put(funcName, imCodes[i].arg1, TableItem(imCodes[i].arg1, "temp", "int"));
			if (imCodes[i].arg2[0] == '@'&&imCodes[i].arg2[1] == 'T')
				table.put(funcName, imCodes[i].arg2, TableItem(imCodes[i].arg2, "temp", "int"));
			if (imCodes[i].arg3[0] == '@'&&imCodes[i].arg3[1] == 'T')
				table.put(funcName, imCodes[i].arg3, TableItem(imCodes[i].arg3, "temp", "int"));
		}
	}
}

Table Optimizer::getTable()
{
	return table;
}


Block::Block()
{

}

Block::Block(int front, int rear, vector<Quadruple> imCodes)
{
	for (int i = front; i < rear; i++)
		codes.push_back(imCodes[i]);
}

CSEliminator::CSEliminator(vector <Block> blocks)
{
	this->blocks = blocks;
}

void CSEliminator::execute()
{
	for (int i = 0; i < blocks.size(); i++)
		executeBlock(i);
}

void CSEliminator::executeBlock(int index)
{
	Block block = blocks[index];
	Block result;
	Dag dag;
	for (int i = 0; i < block.codes.size(); i++)
	{
		string op = block.codes[i].op;
		//op in {+,-,*,/,=}, build dag
		if ((op == "+" || op == "-" || op == "*" || op == "/" || op == "=") &&
			block.codes[i].arg1.find("[")>block.codes[i].arg1.length() &&
			block.codes[i].arg2.find("[")>block.codes[i].arg2.length() &&
			block.codes[i].arg3.find("[") > block.codes[i].arg3.length())
			dag.pushCode(block.codes[i]);
		else //not in {+,-,*,/,=}, export codes from dag, clear dag
		{
			vector<Quadruple> tempCodes = dag.getCodes();
			result.codes.insert(result.codes.end(), tempCodes.begin(), tempCodes.end());
			dag.clear();
			result.codes.push_back(block.codes[i]);
		}
	}
	vector<Quadruple> tempCodes = dag.getCodes();
	result.codes.insert(result.codes.end(), tempCodes.begin(), tempCodes.end());
	blocks[index] = result;
}

vector<Block> CSEliminator::getBlocks()
{
	return blocks;
}

void Dag::pushCode(Quadruple code)
{
	if (code.op == "=")
	{
		int cNode = -1, fNode = -1;
		if (referList.count(code.arg2)) //arg1 is in the list
			cNode = referList[code.arg2];
		else
		{
			DagNode newNode;
			newNode.value = code.arg2;
			newNode.lChild = -1;
			newNode.rChild = -1;
			nodes.push_back(newNode);
			cNode = nodes.size() - 1;
			referList[code.arg2] = cNode;
		}

		int flag = 0;
		for (int i = 0; i < nodes[cNode].fathers.size(); i++)
		{
			if (nodes[nodes[cNode].fathers[i]].value == code.op)
			{
				fNode = nodes[cNode].fathers[i];

				DagNode newNode;
				newNode.value = "=";
				newNode.lChild = fNode;
				newNode.rChild = -1;
				nodes.push_back(newNode);
				nodes[fNode].fathers.push_back(nodes.size() - 1);
				fNode = nodes.size() - 1;
				referList[code.arg1] = fNode;

				flag = 1;
				break;
			}
		}

		if (flag == 0) //creat a father node, update list
		{
			DagNode newNode;
			newNode.value = code.op;
			newNode.lChild = cNode;
			newNode.rChild = -1;
			nodes.push_back(newNode);
			fNode = nodes.size() - 1;
			nodes[cNode].fathers.push_back(fNode);
			referList[code.arg1] = fNode;
		}


	}
	else	//+,-,*,/
	{
		int lNode = -1, rNode = -1, fNode = -1;

		if (referList.count(code.arg1))	//arg1 is in the list
			lNode = referList[code.arg1];
		else	//arg1 is not in the list, create a new node and update the list
		{
			DagNode newNode;
			newNode.value = code.arg1;
			newNode.lChild = -1;
			newNode.rChild = -1;
			nodes.push_back(newNode);
			lNode = nodes.size() - 1;
			referList[code.arg1] = lNode;
		}

		if (referList.count(code.arg2))	//arg2 is in the list
			rNode = referList[code.arg2];
		else	//arg2 is not in the list, create a new node and update the list
		{
			DagNode newNode;
			newNode.value = code.arg2;
			newNode.lChild = -1;
			newNode.rChild = -1;
			nodes.push_back(newNode);
			rNode = nodes.size() - 1;
			referList[code.arg2] = rNode;
		}

		int flag = 0;
		for (int i = 0; i < nodes[lNode].fathers.size(); i++)
		{
			if (nodes[nodes[lNode].fathers[i]].value == code.op)
			{
				fNode = nodes[lNode].fathers[i];
				for (int j = 0; j < nodes[rNode].fathers.size(); j++)
				{
					if (nodes[rNode].fathers[j] == fNode)
					{
						flag = 1;
						break;
					}
				}
			}
			if (flag == 1)
				break;
		}


		if (flag == 1)	//father node exists
		{
			DagNode newNode;
			newNode.value = "=";
			newNode.lChild = fNode;
			newNode.rChild = -1;
			nodes.push_back(newNode);
			nodes[fNode].fathers.push_back(nodes.size() - 1);
			fNode = nodes.size() - 1;
			referList[code.arg3] = fNode;
		}
		else	//creat a father node, update list
		{
			DagNode newNode;
			newNode.value = code.op;
			newNode.lChild = lNode;
			newNode.rChild = rNode;
			nodes.push_back(newNode);
			fNode = nodes.size() - 1;
			nodes[lNode].fathers.push_back(fNode);
			nodes[rNode].fathers.push_back(fNode);
			referList[code.arg3] = fNode;
		}
	}
}

vector<Quadruple> Dag::getCodes()
{
	vector<Quadruple> tempResult;

	vector<bool> bo;
	for (int i = 0; i < nodes.size(); i++)
		bo.push_back(false);

	int count = 0;

	for (int i = 0; i < nodes.size(); i++)
	{
		if (nodes[i].lChild == -1)
		{
			bo[i] = true;
			count++;
		}
	}

	while (count < nodes.size())
	{
		for (int i = 0; i < nodes.size(); i++)
		{
			if (bo[i] == false)
			{
				int flag = 0;
				for (int j = 0; j < nodes[i].fathers.size(); j++)
				{
					if (bo[nodes[i].fathers[j]] == false)
					{
						flag = 1;
						break;
					}
				}
				if (flag == 0)
				{

					int nodeIndex = i;
					while (true)
					{
						if (nodes[nodeIndex].value == "=")
						{
							Quadruple tempCode("=", "", "", "");
							for (map<string, int>::iterator iter = referList.begin(); iter != referList.end(); iter++)
							{
								if (iter->second == nodeIndex)
									tempCode.arg1 = iter->first;

								if (iter->second == nodes[nodeIndex].lChild)
									tempCode.arg2 = iter->first;
							}
							if (tempCode.arg2 == "")
							{
								if (nodes[nodes[nodeIndex].lChild].lChild == -1)
									tempCode.arg2 = nodes[nodes[nodeIndex].lChild].value;
								else
									tempCode.arg2 = "@T" + util::int2string(nodes[nodeIndex].lChild);
							}
							if (tempCode.arg1 == "")
								tempCode.arg1 = "@T" + util::int2string(nodeIndex);

							tempResult.push_back(tempCode);
						}
						else
						{
							Quadruple tempCode(nodes[nodeIndex].value, "", "", "");
							for (map<string, int>::iterator iter = referList.begin(); iter != referList.end(); iter++)
							{
								if (iter->second == nodeIndex)
									tempCode.arg3 = iter->first;

								if (iter->second == nodes[nodeIndex].lChild)
									tempCode.arg1 = iter->first;

								if (iter->second == nodes[nodeIndex].rChild)
									tempCode.arg2 = iter->first;
							}
							if (tempCode.arg1 == "")
							{
								if (nodes[nodes[nodeIndex].lChild].lChild == -1)
									tempCode.arg1 = nodes[nodes[nodeIndex].lChild].value;
								else
									tempCode.arg1 = "@T" + util::int2string(nodes[nodeIndex].lChild);
							}
							if (tempCode.arg2 == "")
							{
								if (nodes[nodes[nodeIndex].rChild].lChild == -1)
									tempCode.arg2 = nodes[nodes[nodeIndex].rChild].value;
								else
									tempCode.arg2 = "@T" + util::int2string(nodes[nodeIndex].rChild);
							}
							if (tempCode.arg3 == "")
								tempCode.arg3 = "@T" + util::int2string(nodeIndex);

							tempResult.push_back(tempCode);
						}

						bo[nodeIndex] = true;
						count++;
						nodeIndex = nodes[nodeIndex].lChild;

						if (bo[nodeIndex] == true)
							break;
						flag = 0;
						for (int j = 0; j < nodes[nodeIndex].fathers.size(); j++)
						{
							if (bo[nodes[nodeIndex].fathers[j]] == false)
							{
								flag = 1;
								break;
							}
						}
						if (flag == 1)
							break;
					}

				}
			}
		}
	}

	vector<Quadruple> result;
	for (int i = tempResult.size() - 1; i >= 0; i--)
		result.push_back(tempResult[i]);
	return result;
}

void Dag::clear()
{
	nodes.clear();
	referList.clear();
}