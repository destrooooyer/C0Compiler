#ifndef TABLE_H
#define TABLE_H

#include <map>
#include <vector>
#include <string>


class TableItem
{
public:
	TableItem();
	TableItem(std::string name, std::string kind, std::string type);
	void setValue(int value);
	int getValue();
	std::string getName();
	std::string getKind();
	std::string getType();

private:
	std::string name;
	std::string kind;
	std::string type;
	int value;
	int addr;
};

class Table
{
public:
	Table();
	void put(std::string funcName, std::string name, TableItem item);
	TableItem get(std::string funcName, std::string name);
	bool check(std::string functionName, std::string name);
	void newTable(std::string funcName);
	void print();
	std::map<std::string, TableItem> getTable(std::string funcName);

private:
	//tables: 
	//		each function has its respective table
	//		function name is the key of this map
	//	table:
	//			key: name of identifier
	//			value: tableItem
	std::map<std::string, std::map<std::string, TableItem> > tables;
};

#endif 
