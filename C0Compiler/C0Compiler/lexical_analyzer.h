#ifndef LEXICAL_ANALYZER_H
#define LEXICAL_ANALYZER_H

#include <fstream>
#include <string>
#include <map>

class Symbol
{
public:
	Symbol();
	Symbol(std::string type);
	Symbol(std::string type, std::string value,int lineNumber);
	std::string getValue();
	std::string getType();
	int getLineNumber();

private:
	std::string value;
	std::string type;
	int lineNumber;
};


class LexAnalyzer
{
public:
	LexAnalyzer(std::string soucrFile);
	bool isSourceOpen();
	Symbol getNextSymble();
	bool infoSwitch;

private:
	int lineCount;
	char chr;
	std::ifstream fin;
	std::map <std::string, std::string> preservedWordsTable;
	void printInfo(Symbol sym);
};

#endif