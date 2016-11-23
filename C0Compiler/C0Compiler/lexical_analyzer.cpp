#include "lexical_analyzer.h"
#include "util.h"
#include <string>
#include <iostream>
#include <iomanip>
using namespace std;


LexAnalyzer::LexAnalyzer(string sourceFile)
{
	this->fin.open(sourceFile);
	this->lineCount = 1;			//lineCount begins with 1
	this->chr = ' ';				//set chr to ' ' for the reason that whitespace has no meaning
	this->infoSwitch = false;		//to print info or not

	//initial the preserved word table
	this->preservedWordsTable["const"] = "PRESERVED_WORD_CONST";
	this->preservedWordsTable["int"] = "PRESERVED_WORD_INT";
	this->preservedWordsTable["char"] = "PRESERVED_WORD_CHAR";
	this->preservedWordsTable["void"] = "PRESERVED_WORD_VOID";
	this->preservedWordsTable["main"] = "PRESERVED_WORD_MAIN";
	this->preservedWordsTable["return"] = "PRESERVED_WORD_RETURN";
	this->preservedWordsTable["printf"] = "PRESERVED_WORD_PRINTF";
	this->preservedWordsTable["scanf"] = "PRESERVED_WORD_SCANF";
	this->preservedWordsTable["if"] = "PRESERVED_WORD_IF";
	this->preservedWordsTable["else"] = "PRESERVED_WORD_ELSE";
	this->preservedWordsTable["do"] = "PRESERVED_WORD_DO";
	this->preservedWordsTable["while"] = "PRESERVED_WORD_WHILE";
	this->preservedWordsTable["for"] = "PRESERVED_WORD_FOR";
}

//check whether the source file is successfully opened
bool LexAnalyzer::isSourceOpen()
{
	return fin.is_open();
}

void LexAnalyzer::printInfo(Symbol sym)
{
	int n = 0, m = lineCount;
	while (m != 0)
	{
		m /= 10;
		n++;
	}
	if (this->infoSwitch)
		cout << "line[" << lineCount << "]" << setw(11 - n) << "type: " << sym.getType() << setw(30 - sym.getType().length()) << "value: " << sym.getValue() << "\n";
}

Symbol LexAnalyzer::getNextSymble()
{
	//check eof first
	if (fin.eof())
	{
		chr = -1;
		Symbol sym("EOF");
		return sym;
	}

	//skip all whitespaces
	while (chr == ' ' || chr == '\t' || chr == '\n')
	{
		if (chr == '\n')	//maintain lineCount
			lineCount++;

		if (fin.peek() == EOF)
		{
			chr = -1;
			Symbol sym("EOF");
			return sym;
		}
		else
			fin.get(chr);
	}

	//identifier or preserved word
	if (util::isLetter(chr) || chr == '_')
	{
		string valueTmp;

		while (util::isLetter(chr) || chr == '_' || util::isNumber(chr))
		{
			valueTmp += chr;
			if (fin.peek() == EOF)
			{
				chr = -1;
				break;
			}
			else
				fin.get(chr);
		}

		//check whether it is a preserved word
		if (preservedWordsTable.count(valueTmp) == 1)	//a preserved word
		{
			Symbol sym(preservedWordsTable[valueTmp], valueTmp, lineCount);
			printInfo(sym);
			return sym;
		}
		else	//an identifier
		{
			Symbol sym("IDENTIFIER", valueTmp, lineCount);
			printInfo(sym);
			return sym;
		}
	}
	//unsigned int
	else if (util::isNumber(chr) && chr != '0')
	{
		string valueTmp;

		while (util::isNumber(chr))
		{
			valueTmp += chr;
			if (fin.peek() == EOF)
			{
				chr = -1;
				break;
			}
			else
				fin.get(chr);
		}

		Symbol sym("UNSIGNEDINT", valueTmp, lineCount);
		printInfo(sym);
		return sym;
	}
	//string
	else if (chr == '\"')
	{
		string valueTmp;

		valueTmp += chr;
		fin.get(chr);
		while (util::isChr(chr))
		{
			valueTmp += chr;
			if (fin.peek() == EOF)
			{
				chr = -1;
				break;
			}
			else
				fin.get(chr);
		}
		if (chr == '\"')
		{
			valueTmp += chr;
			fin.get(chr);
			Symbol sym("STRING", valueTmp, lineCount);
			printInfo(sym);
			return sym;
		}
		else
		{
			Symbol sym("UNKNOWN", valueTmp, lineCount);
			printInfo(sym);
			return sym;
		}
	}
	//char
	else if (chr == '\'')
	{
		string valueTmp;

		valueTmp += chr;
		fin.get(chr);

		if (chr == '+' || chr == '-' || chr == '*' || chr == '/' || chr == '_' || util::isLetter(chr) || util::isNumber(chr))
		{
			valueTmp += chr;
			fin.get(chr);
		}
		else
		{
			Symbol sym("UNKNOWN", valueTmp, lineCount);
			printInfo(sym);
			return sym;
		}

		if (chr == '\'')
		{
			valueTmp += chr;
			fin.get(chr);
			Symbol sym("CHAR", valueTmp, lineCount);
			printInfo(sym);
			return sym;
		}
		else
		{
			Symbol sym("UNKNOWN", valueTmp, lineCount);
			printInfo(sym);
			return sym;
		}
	}
	//others
	else if (chr == '+')
	{
		fin.get(chr);
		Symbol sym("PLUS", "+", lineCount);
		printInfo(sym);
		return sym;
	}
	else if (chr == '-')
	{
		fin.get(chr);
		Symbol sym("MINUS", "-", lineCount);
		printInfo(sym);
		return sym;
	}
	else if (chr == '*')
	{
		fin.get(chr);
		Symbol sym("TIMES", "*", lineCount);
		printInfo(sym);
		return sym;
	}
	else if (chr == '/')
	{
		fin.get(chr);
		Symbol sym("SLASH", "/", lineCount);
		printInfo(sym);
		return sym;
	}
	else if (chr == '(')
	{
		fin.get(chr);
		Symbol sym("LPAREN", "(", lineCount);
		printInfo(sym);
		return sym;
	}
	else if (chr == ')')
	{
		fin.get(chr);
		Symbol sym("RPAREN", ")", lineCount);
		printInfo(sym);
		return sym;
	}
	else if (chr == '{')
	{
		fin.get(chr);
		Symbol sym("LBRACE", "{", lineCount);
		printInfo(sym);
		return sym;
	}
	else if (chr == '}')
	{
		fin.get(chr);
		Symbol sym("RBRACE", "}", lineCount);
		printInfo(sym);
		return sym;
	}
	else if (chr == '[')
	{
		fin.get(chr);
		Symbol sym("LBRACKET", "[", lineCount);
		printInfo(sym);
		return sym;
	}
	else if (chr == ']')
	{
		fin.get(chr);
		Symbol sym("RBRACKET", "]", lineCount);
		printInfo(sym);
		return sym;
	}
	else if (chr == '=')
	{
		fin.get(chr);

		if (chr == '=')
		{
			fin.get(chr);
			Symbol sym("EQUAL", "==", lineCount);
			printInfo(sym);
			return sym;
		}
		else
		{
			Symbol sym("ASSIGN", "=", lineCount);
			printInfo(sym);
			return sym;
		}
	}
	else if (chr == ',')
	{
		fin.get(chr);
		Symbol sym("COMMA", ",", lineCount);
		printInfo(sym);
		return sym;
	}
	else if (chr == ';')
	{
		fin.get(chr);
		Symbol sym("SEMICOLON", ";", lineCount);
		printInfo(sym);
		return sym;
	}
	else if (chr == '<')
	{
		fin.get(chr);

		if (chr == '=')
		{
			fin.get(chr);
			Symbol sym("LESS_OR_EQUAL", "<=", lineCount);
			printInfo(sym);
			return sym;
		}
		else
		{
			Symbol sym("LESS", "<", lineCount);
			printInfo(sym);
			return sym;
		}
	}
	else if (chr == '>')
	{
		fin.get(chr);

		if (chr == '=')
		{
			fin.get(chr);
			Symbol sym("GREATER_OR_EQUAL", ">=", lineCount);
			printInfo(sym);
			return sym;
		}
		else
		{
			Symbol sym("GREATER", ">", lineCount);
			printInfo(sym);
			return sym;
		}
	}
	else if (chr == '!')
	{
		fin.get(chr);
		if (chr == '=')
		{
			fin.get(chr);
			Symbol sym("UNEQUAL", "!=", lineCount);
			printInfo(sym);
			return sym;
		}
		else
		{
			Symbol sym("UNKNOWN", "!" + chr, lineCount);
			fin.get(chr);
			printInfo(sym);
			return sym;
		}
	}
	else if (chr == '0')
	{
		fin.get(chr);
		Symbol sym("ZERO", "0", lineCount);
		printInfo(sym);
		return sym;
	}
	else
	{
		Symbol sym("UNKNOWN", "" + chr, lineCount);
		fin.get(chr);
		printInfo(sym);
		return sym;
	}

}

Symbol::Symbol()
{
}

Symbol::Symbol(string type)
{
	this->type = type;
}

Symbol::Symbol(string type, string value, int lineNumber)
{
	this->type = type;
	this->value = value;
	this->lineNumber = lineNumber;
}

string Symbol::getType()
{
	return this->type;
}

string Symbol::getValue()
{
	return this->value;
}

int Symbol::getLineNumber()
{
	return this->lineNumber;
}