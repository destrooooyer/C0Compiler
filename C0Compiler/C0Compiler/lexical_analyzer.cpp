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

void LexAnalyzer::printInfo(Symble sym)
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

Symble LexAnalyzer::getNextSymble()
{
	//check eof first
	if (fin.peek() == EOF)
	{
		chr = -1;
		Symble sym("EOF");
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
			Symble sym("EOF");
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
			Symble sym(preservedWordsTable[valueTmp], valueTmp, lineCount);
			printInfo(sym);
			return sym;
		}
		else	//an identifier
		{
			Symble sym("IDENTIFIER", valueTmp, lineCount);
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

		Symble sym("UNSIGNEDINT", valueTmp, lineCount);
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
			Symble sym("STRING", valueTmp, lineCount);
			printInfo(sym);
			return sym;
		}
		else
		{
			Symble sym("UNKNOWN", valueTmp, lineCount);
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
			Symble sym("UNKNOWN", valueTmp, lineCount);
			printInfo(sym);
			return sym;
		}

		if (chr == '\'')
		{
			valueTmp += chr;
			fin.get(chr);
			Symble sym("CHAR", valueTmp, lineCount);
			printInfo(sym);
			return sym;
		}
		else
		{
			Symble sym("UNKNOWN", valueTmp, lineCount);
			printInfo(sym);
			return sym;
		}
	}
	//others
	else if (chr == '+')
	{
		fin.get(chr);
		Symble sym("PLUS", "+", lineCount);
		printInfo(sym);
		return sym;
	}
	else if (chr == '-')
	{
		fin.get(chr);
		Symble sym("MINUS", "-", lineCount);
		printInfo(sym);
		return sym;
	}
	else if (chr == '*')
	{
		fin.get(chr);
		Symble sym("TIMES", "*", lineCount);
		printInfo(sym);
		return sym;
	}
	else if (chr == '/')
	{
		fin.get(chr);
		Symble sym("SLASH", "/", lineCount);
		printInfo(sym);
		return sym;
	}
	else if (chr == '(')
	{
		fin.get(chr);
		Symble sym("LPAREN", "(", lineCount);
		printInfo(sym);
		return sym;
	}
	else if (chr == ')')
	{
		fin.get(chr);
		Symble sym("RPAREN", ")", lineCount);
		printInfo(sym);
		return sym;
	}
	else if (chr == '{')
	{
		fin.get(chr);
		Symble sym("LBRACE", "{", lineCount);
		printInfo(sym);
		return sym;
	}
	else if (chr == '}')
	{
		fin.get(chr);
		Symble sym("RBRACE", "}", lineCount);
		printInfo(sym);
		return sym;
	}
	else if (chr == '[')
	{
		fin.get(chr);
		Symble sym("LBRACKET", "[", lineCount);
		printInfo(sym);
		return sym;
	}
	else if (chr == ']')
	{
		fin.get(chr);
		Symble sym("RBRACKET", "]", lineCount);
		printInfo(sym);
		return sym;
	}
	else if (chr == '=')
	{
		fin.get(chr);

		if (chr == '=')
		{
			fin.get(chr);
			Symble sym("EQUAL", "==", lineCount);
			printInfo(sym);
			return sym;
		}
		else
		{
			Symble sym("ASSIGN", "=", lineCount);
			printInfo(sym);
			return sym;
		}
	}
	else if (chr == ',')
	{
		fin.get(chr);
		Symble sym("COMMA", ",", lineCount);
		printInfo(sym);
		return sym;
	}
	else if (chr == ';')
	{
		fin.get(chr);
		Symble sym("SEMICOLON", ";", lineCount);
		printInfo(sym);
		return sym;
	}
	else if (chr == '<')
	{
		fin.get(chr);

		if (chr == '=')
		{
			fin.get(chr);
			Symble sym("LESS_OR_EQUAL", "<=", lineCount);
			printInfo(sym);
			return sym;
		}
		else
		{
			Symble sym("LESS", "<", lineCount);
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
			Symble sym("GREATER_OR_EQUAL", ">=", lineCount);
			printInfo(sym);
			return sym;
		}
		else
		{
			Symble sym("GREATER", ">", lineCount);
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
			Symble sym("UNEQUAL", "!=", lineCount);
			printInfo(sym);
			return sym;
		}
		else
		{
			Symble sym("UNKNOWN", "!" + chr, lineCount);
			fin.get(chr);
			printInfo(sym);
			return sym;
		}
	}
	else if (chr == '0')
	{
		fin.get(chr);
		Symble sym("ZERO", "0", lineCount);
		printInfo(sym);
		return sym;
	}
	else
	{
		Symble sym("UNKNOWN", "" + chr, lineCount);
		fin.get(chr);
		printInfo(sym);
		return sym;
	}

}



Symble::Symble(string type)
{
	this->type = type;
}

Symble::Symble(string type, string value, int lineNumber)
{
	this->type = type;
	this->value = value;
	this->lineNumber = lineNumber;
}

string Symble::getType()
{
	return this->type;
}

string Symble::getValue()
{
	return this->value;
}

int Symble::getLineNumber()
{
	return this->lineNumber;
}