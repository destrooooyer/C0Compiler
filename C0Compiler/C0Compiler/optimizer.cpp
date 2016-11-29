#include "optimizer.h"
#include "util.h"
using namespace std;
Optimizer::Optimizer(vector<Quadruple> imCodes, Table table)
{
	this->imCodes = imCodes;
	this->table = table;
}

void Optimizer::optimize()
{
	eliminateConsts();
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
			tempCodes[i].op != "callvoid")
		{
			string temp = tempCodes[i].arg1;
			if (table.check(funcName, temp) && table.get(funcName, temp).getKind() == "const")
				tempCodes[i].arg1 = util::int2string(table.get(funcName, temp).getValue());
			else if (table.check("globle", temp) && table.get("globle", temp).getKind() == "const")
				tempCodes[i].arg1 = util::int2string(table.get("globle", temp).getValue());

			temp = tempCodes[i].arg2;
			if (table.check(funcName, temp) && table.get(funcName, temp).getKind() == "const")
				tempCodes[i].arg2 = table.get(funcName, temp).getValue();
			else if (table.check("globle", temp) && table.get("globle", temp).getKind() == "const")
				tempCodes[i].arg2 = util::int2string(table.get("globle", temp).getValue());

			temp = tempCodes[i].arg3;
			if (table.check(funcName, temp) && table.get(funcName, temp).getKind() == "const")
				tempCodes[i].arg3 = table.get(funcName, temp).getValue();
			else if (table.check("globle", temp) && table.get("globle", temp).getKind() == "const")
				tempCodes[i].arg3 = util::int2string(table.get("globle", temp).getValue());
		}
	}
	imCodes = tempCodes;

}