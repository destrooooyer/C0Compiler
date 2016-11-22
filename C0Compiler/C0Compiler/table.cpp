#include "table.h"
#include<iostream>
using namespace std;

TableItem::TableItem(string name, string kind, string type)
{
	this->name = name;
	this->kind = kind;
	this->type = type;
	this->value = 0;
}

TableItem::TableItem()
{
	this->value = 0;
}

void TableItem::setValue(int value)
{
	this->value = value;
}

int TableItem::getValue()
{
	return this->value;
}

string TableItem::getType()
{
	return this->type;
}

string TableItem::getName()
{
	return this->name;
}

string TableItem::getKind()
{
	return this->kind;
}

Table::Table()
{
	//create the "globle" table when is inited
	map<string, TableItem> tableGloble;
	tables["globle"] = tableGloble;
}


bool Table::check(string funcName, string name)
{
	if (tables.count(funcName) && tables[funcName].count(name))
	{
		return true;
	}
	else
		return false;
}

void Table::put(string funcName, string name, TableItem item)
{
	tables[funcName][name] = item;
}

void Table::newTable(string funName)
{
	if (!tables.count(funName))
	{
		map<string, TableItem> tableNew;
		tables[funName] = tableNew;
	}
}

TableItem Table::get(string funName, string name)
{
	if (tables.count(funName) && tables[funName].count(name))
		return tables[funName][name];
}


void Table::print()
{
	for (map<string, map<string, TableItem> >::iterator iter = tables.begin(); iter != tables.end(); iter++)
	{
		cout << "///////////////////////////////////////////\n";
		cout << iter->first<<endl;
		for (map<string, TableItem>::iterator iter2 = iter->second.begin(); iter2 != iter->second.end(); iter2++)
		{
			cout << iter2->second.getName() << " " << iter2->second.getKind() << " " << iter2->second.getType() << " " << iter2->second.getValue() << endl;
		}
	}
}
