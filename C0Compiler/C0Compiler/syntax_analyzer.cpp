#include "syntax_analyzer.h"
#include "lexical_analyzer.h"
#include <iostream>
#include <vector>

using namespace std;

SyntaxAnalyzer::SyntaxAnalyzer(vector<Symbol> symbles)
{
	this->symbles = symbles;
	this->iter = 0;
}

void SyntaxAnalyzer::printInfo(string str)
{
	if (infoSwitch)
		cout << str << endl;
}

vector<Quadruple> SyntaxAnalyzer::getIntermediateCodes()
{
	return imCodeGenerator.getQuadruples();
}


//＜程序＞    :: = ［＜常量说明＞］［＜变量说明＞］{ ＜有返回值函数定义＞ | ＜无返回值函数定义＞ }＜主函数＞
bool SyntaxAnalyzer::program()
{
	funcName = "globle";
	//［＜常量说明＞］
	if (symbles[iter].getType() == "PRESERVED_WORD_CONST")		//const
		constExplanation();

	//［＜变量说明＞］or ＜有返回值函数定义＞
	if (symbles[iter].getType() == "PRESERVED_WORD_INT" ||		//int
		symbles[iter].getType() == "PRESERVED_WORD_CHAR")		//char
	{
		int iterBackUp = iter;	//create a back up for iter

		iter++;
		if (symbles[iter].getType() == "IDENTIFIER")
		{
			iter++;
			if (symbles[iter].getType() == "LPAREN")	//it should be ＜有返回值函数定义＞
			{
				iter = iterBackUp;	//recover iter;
				returnFunctionDeclaration();
			}
			else	//it should be ＜变量说明＞
			{
				iter = iterBackUp;
				varExplanation();
			}

		}
		else //error, there should be an identifier
		{
			_error(1, symbles[iter].getLineNumber());
		}
	}

	// 	//＜无返回值函数定义＞ or ＜主函数＞
	// 	if (symbles[iter].getType() == "PRESERVED_WORD_VOID")		//void
	// 	{
	// 		int iterBackUp = iter;	//create a back up for iter
	// 
	// 		iter++;
	// 		if (symbles[iter].getType() == "PRESERVED_WORD_MAIN")
	// 		{
	// 			iter = iterBackUp;
	// 			mainFunction();
	// 			printInfo("This is a ＜程序＞");
	// 			return true;
	// 		}
	// 		else //void function
	// 		{
	// 			iter = iterBackUp;
	// 			voidFunctionDeclaration();
	// 		}
	// 	}


		//{ ＜有返回值函数定义＞ | ＜无返回值函数定义＞ }| ＜主函数＞
	while (symbles[iter].getType() == "PRESERVED_WORD_INT" ||	//int
		symbles[iter].getType() == "PRESERVED_WORD_CHAR" ||		//char
		symbles[iter].getType() == "PRESERVED_WORD_VOID")		//void
	{
		//＜无返回值函数定义＞ or ＜主函数＞
		if (symbles[iter].getType() == "PRESERVED_WORD_VOID")		//void
		{
			int iterBackUp = iter;	//create a back up for iter

			iter++;
			if (symbles[iter].getType() == "PRESERVED_WORD_MAIN")
			{
				iter = iterBackUp;
				mainFunction();
				break;
			}
			else //void function
			{
				iter = iterBackUp;
				voidFunctionDeclaration();
			}
		}
		//＜有返回值函数定义＞
		else if (symbles[iter].getType() == "PRESERVED_WORD_INT" ||
			symbles[iter].getType() == "PRESERVED_WORD_CHAR")
		{
			returnFunctionDeclaration();
		}
	}

	printInfo("This is a ＜程序＞");
	return true;
}

//＜常量说明＞ ::=  const＜常量定义＞;{ const＜常量定义＞;}
bool SyntaxAnalyzer::constExplanation()
{
	//const
	if (symbles[iter].getType() == "PRESERVED_WORD_CONST")
	{
		iter++;
		//＜常量定义＞
		constDeclaration();

		//;
		if (symbles[iter].getType() == "SEMICOLON")
		{
			iter++;
			//{ const＜常量定义＞; }
			while (symbles[iter].getType() == "PRESERVED_WORD_CONST")	//while next symble is "const"
			{
				iter++;
				//＜常量定义＞
				constDeclaration();

				//;
				if (symbles[iter].getType() == "SEMICOLON")
					iter++;
				else	//; not found, error
				{
					_error(4, symbles[iter].getLineNumber());
				}
			}

		}
		else	//; not found, error
		{
			_error(4, symbles[iter].getLineNumber());
		}
	}
	else	//const not found, error
	{
		_error(3, symbles[iter].getLineNumber());
	}

	printInfo("This is a ＜常量说明＞");
	return true;
}

//＜常量定义＞   ::=   int＜标识符＞＝＜整数＞{,＜标识符＞＝＜整数＞} | char＜标识符＞＝＜字符＞{ ,＜标识符＞＝＜字符＞ }
bool SyntaxAnalyzer::constDeclaration()
{
	string name;
	string type;
	TableItem tableItem;
	int value;

	int iterBackUp = iter;	//create a backup for iter

	//int
	if (symbles[iter].getType() == "PRESERVED_WORD_INT")
	{
		type = "int";
		iter++;
		if (symbles[iter].getType() == "IDENTIFIER")
		{
			name = symbles[iter].getValue();
			if (table.check(funcName, name))	//redeclaration, error
			{
				_error(26, symbles[iter].getLineNumber());
			}
			iter++;
			if (symbles[iter].getType() == "ASSIGN")
			{
				iter++;
				//＜整数＞
				value = integer();
				tableItem = TableItem(name, "const", type);
				tableItem.setValue(value);
				table.put(funcName, name, tableItem);

				while (symbles[iter].getType() == "COMMA")
				{
					iter++;
					if (symbles[iter].getType() == "IDENTIFIER")
					{
						name = symbles[iter].getValue();
						if (table.check(funcName, name))	//redeclaration, error
						{
							_error(26, symbles[iter].getLineNumber());
						}
						iter++;
						if (symbles[iter].getType() == "ASSIGN")
						{
							iter++;
							//＜整数＞
							value = integer();
							tableItem = TableItem(name, "const", type);
							tableItem.setValue(value);
							table.put(funcName, name, tableItem);
						}
						else	//= not found, error
						{
							_error(6, symbles[iter].getLineNumber());
						}
					}
					else	//identifier not found, error
					{
						_error(1, symbles[iter].getLineNumber());
					}
				}

			}
			else	//= not found, error
			{
				_error(6, symbles[iter].getLineNumber());
			}
		}
		else	//identifier not found, error
		{
			_error(1, symbles[iter].getLineNumber());
		}
	}
	//char
	else if (symbles[iter].getType() == "PRESERVED_WORD_CHAR")
	{
		type = "char";
		iter++;
		if (symbles[iter].getType() == "IDENTIFIER")
		{
			name = symbles[iter].getValue();
			if (table.check(funcName, name))	//redeclaration, error
			{
				_error(26, symbles[iter].getLineNumber());
			}
			iter++;
			if (symbles[iter].getType() == "ASSIGN")
			{
				iter++;
				//＜字符＞
				value = _char();
				tableItem = TableItem(name, "const", type);
				tableItem.setValue(value);
				table.put(funcName, name, tableItem);

				while (symbles[iter].getType() == "COMMA")
				{
					iter++;
					if (symbles[iter].getType() == "IDENTIFIER")
					{
						name = symbles[iter].getValue();
						if (table.check(funcName, name))	//redeclaration, error
						{
							_error(26, symbles[iter].getLineNumber());
						}
						iter++;
						if (symbles[iter].getType() == "ASSIGN")
						{
							iter++;
							//＜字符＞
							value = _char();
							tableItem = TableItem(name, "const", type);
							tableItem.setValue(value);
							table.put(funcName, name, tableItem);
						}
						else	//= not found, error
						{
							_error(6, symbles[iter].getLineNumber());
						}
					}
					else	//identifier not found, error
					{
						_error(1, symbles[iter].getLineNumber());
					}
				}

			}
			else	//= not found, error
			{
				_error(6, symbles[iter].getLineNumber());
			}
		}
		else	//identifier not found, error
		{
			_error(1, symbles[iter].getLineNumber());
		}
	}
	else	//type identifier not found, error
	{
		_error(5, symbles[iter].getLineNumber());
	}

	printInfo("This is a ＜常量定义＞");
	return true;
}

//＜整数＞        ::= ［＋｜－］＜无符号整数＞｜０
int SyntaxAnalyzer::integer()
{
	int factor = 1;			//this decides whether the int is positive
	int value = 0;
	if (symbles[iter].getType() == "PLUS" ||
		symbles[iter].getType() == "MINUS")
	{
		if (symbles[iter].getType() == "MINUS")
			factor = -1;	//negative
		iter++;
	}
	if (symbles[iter].getType() == "UNSIGNEDINT" ||
		symbles[iter].getType() == "ZERO")
	{
		if (symbles[iter].getType() == "ZERO")
			value = 0;
		else
			value = util::parseInt(symbles[iter].getValue());
		iter++;
	}
	else	//integer, error
	{
		_error(9, symbles[iter].getLineNumber());
	}
	printInfo("This is a <整数>");
	return value*factor;
}

//＜字符＞    ::=  '＜加法运算符＞'｜'＜乘法运算符＞'｜'＜字母＞'｜'＜数字＞'
char SyntaxAnalyzer::_char()
{
	char temp = 0;
	if (symbles[iter].getType() == "CHAR")
	{
		temp = util::parseChar(symbles[iter].getValue());
		iter++;
	}
	else	//char not found, error
	{
		_error(10, symbles[iter].getLineNumber());
	}

	printInfo("This is a <字符>");
	return temp;
}

//＜变量说明＞  ::= ＜变量定义＞;{＜变量定义＞;}
bool SyntaxAnalyzer::varExplanation()
{
	varDeclaration();
	if (symbles[iter].getType() == "SEMICOLON")
	{
		iter++;
		//{＜变量定义＞;}
		while (true)
		{
			//＜变量定义＞or ＜有返回值函数定义＞
			if (symbles[iter].getType() == "PRESERVED_WORD_INT" ||		//int
				symbles[iter].getType() == "PRESERVED_WORD_CHAR")		//char
			{
				int iterBackUp = iter;	//create a back up for iter

				iter++;
				if (symbles[iter].getType() == "IDENTIFIER")
				{
					iter++;
					if (symbles[iter].getType() != "LPAREN")	//it is not ＜有返回值函数定义＞, so it is＜变量定义＞
					{
						iter = iterBackUp;	//recover iter;
						varDeclaration();
						if (symbles[iter].getType() == "SEMICOLON")
							iter++;
						else	//; not found, error
						{
							_error(4, symbles[iter].getLineNumber());
						}
					}
					else	//it should be ＜有返回值函数定义＞
					{
						iter = iterBackUp;
						break;
					}

				}
				else //error, there should be an identifier
				{
					_error(1, symbles[iter].getLineNumber());
				}
			}
			else
				break;
		}
	}
	else	//; not found, error
	{
		_error(4, symbles[iter].getLineNumber());
	}


	printInfo("This is a ＜变量说明＞");
	return true;
}

//＜变量定义＞  ::= ＜类型标识符＞(＜标识符＞|＜标识符＞‘[’＜无符号整数＞‘]’){,(＜标识符＞|＜标识符＞‘[’＜无符号整数＞‘]’) }
bool SyntaxAnalyzer::varDeclaration()
{
	string name;
	string type;
	string kind;
	TableItem tableItem;
	int value;

	//＜类型标识符＞
	type = typeIdentifier();

	//＜标识符＞
	if (symbles[iter].getType() == "IDENTIFIER")
	{
		name = symbles[iter].getValue();
		if (table.check(funcName, name))	//redeclaration, error
		{
			_error(26, symbles[iter].getLineNumber());
		}
		kind = "variable";
		value = 0;
		iter++;
		//‘[’＜无符号整数＞‘]’
		if (symbles[iter].getType() == "LBRACKET")
		{
			iter++;
			if (symbles[iter].getType() == "UNSIGNEDINT")
			{
				value = util::parseInt(symbles[iter].getValue());
				iter++;
				if (symbles[iter].getType() == "RBRACKET")
				{
					iter++;
					kind = "array";		//this is an array
				}
				else	//] not found, error
				{
					_error(8, symbles[iter].getLineNumber());
				}
			}
			else	//unsigned int not found, error
			{
				_error(7, symbles[iter].getLineNumber());
			}
		}

		tableItem = TableItem(name, kind, type);
		tableItem.setValue(value);
		table.put(funcName, name, tableItem);

		//{,(＜标识符＞|＜标识符＞‘[’＜无符号整数＞‘]’) }
		while (symbles[iter].getType() == "COMMA")
		{
			iter++;
			//＜标识符＞
			if (symbles[iter].getType() == "IDENTIFIER")
			{
				name = symbles[iter].getValue();
				if (table.check(funcName, name))	//redeclaration, error
				{
					_error(26, symbles[iter].getLineNumber());
				}
				kind = "variable";
				value = 0;
				iter++;

				//‘[’＜无符号整数＞‘]’
				if (symbles[iter].getType() == "LBRACKET")
				{
					iter++;
					if (symbles[iter].getType() == "UNSIGNEDINT")
					{
						value = util::parseInt(symbles[iter].getValue());
						iter++;
						if (symbles[iter].getType() == "RBRACKET")
						{
							iter++;
							kind = "array";		//this is an array
						}
						else	//] not found, error
						{
							_error(8, symbles[iter].getLineNumber());
						}
					}
					else	//unsigned int not found, error
					{
						_error(7, symbles[iter].getLineNumber());
					}
				}

				tableItem = TableItem(name, kind, type);
				tableItem.setValue(value);
				table.put(funcName, name, tableItem);
			}
			else	//identifier not found, error
			{
				_error(1, symbles[iter].getLineNumber());
			}
		}
	}
	else	//identifier not found, error
	{
		_error(1, symbles[iter].getLineNumber());
	}


	printInfo("This is a ＜变量定义＞");
	return true;
}

//＜类型标识符＞      ::=  int | char
string SyntaxAnalyzer::typeIdentifier()
{
	string temp;
	if (symbles[iter].getType() == "PRESERVED_WORD_INT" ||
		symbles[iter].getType() == "PRESERVED_WORD_CHAR")
	{
		temp = symbles[iter].getValue();
		iter++;
	}
	else	//type identifier not found,error
	{
		_error(5, symbles[iter].getLineNumber());
	}

	printInfo("This is a ＜类型标识符＞");
	return temp;
}

//＜有返回值函数定义＞  ::=  ＜声明头部＞‘(’＜参数＞‘)’ ‘{’＜复合语句＞‘}’
bool SyntaxAnalyzer::returnFunctionDeclaration()
{
	string name;
	string type;
	string kind = "return_function";
	TableItem tableItem;
	int value;

	//int or char
	if (symbles[iter].getType() == "PRESERVED_WORD_INT" ||
		symbles[iter].getType() == "PRESERVED_WORD_CHAR")
	{
		type = symbles[iter].getValue();
		iter++;
		//identifier
		if (symbles[iter].getType() == "IDENTIFIER")
		{
			funcName = symbles[iter].getValue();
			name = funcName;
			table.newTable(name);
			iter++;
			//‘(’＜参数＞‘)’
			if (symbles[iter].getType() == "LPAREN")		//(
			{
				iter++;
				value = parameter();	//＜参数＞
				if (symbles[iter].getType() == "RPAREN")	//)
				{
					tableItem = TableItem(name, kind, type);
					tableItem.setValue(value);
					table.put("globle", name, tableItem);
					iter++;
					if (symbles[iter].getType() == "LBRACE")	//{
					{
						iter++;

						imCodeGenerator.genProc(name);
						compondStatement();
						imCodeGenerator.genEndp(name);

						if (symbles[iter].getType() == "RBRACE")	//}
						{
							iter++;
						}
						else	//} not found, error
						{
							_error(13, symbles[iter].getLineNumber());
						}
					}
					else	//{ not found, error
					{
						_error(12, symbles[iter].getLineNumber());
					}
				}
				else	//) not found, error
				{
					_error(11, symbles[iter].getLineNumber());
				}
			}
			else	//( not found, error
			{
				_error(2, symbles[iter].getLineNumber());
			}
		}
		else	//identifier not found, error
		{
			_error(1, symbles[iter].getLineNumber());
		}

	}
	else	//type identifier not found, error
	{
		_error(5, symbles[iter].getLineNumber());
	}

	printInfo("This is a ＜有返回值函数定义＞");
	return true;
}


//＜无返回值函数定义＞  ::= void＜标识符＞‘(’＜参数＞‘)’‘{’＜复合语句＞‘}’
bool SyntaxAnalyzer::voidFunctionDeclaration()
{
	string name;
	string type = "void";
	string kind = "void_function";
	TableItem tableItem;
	int value;
	//void
	if (symbles[iter].getType() == "PRESERVED_WORD_VOID")
	{
		iter++;
		//identifier
		if (symbles[iter].getType() == "IDENTIFIER")
		{
			funcName = symbles[iter].getValue();
			name = funcName;
			table.newTable(name);
			iter++;
			//‘(’＜参数＞‘)’
			if (symbles[iter].getType() == "LPAREN")		//(
			{
				iter++;
				value = parameter();	//＜参数＞
				if (symbles[iter].getType() == "RPAREN")	//)
				{
					tableItem = TableItem(name, kind, type);
					tableItem.setValue(value);
					table.put("globle", name, tableItem);
					iter++;
					if (symbles[iter].getType() == "LBRACE")	//{
					{
						iter++;

						imCodeGenerator.genProc(name);
						compondStatement();
						imCodeGenerator.genEndp(name);

						if (symbles[iter].getType() == "RBRACE")	//}
						{
							iter++;
						}
						else	//} not found, error
						{
							_error(13, symbles[iter].getLineNumber());
						}
					}
					else	//{ not found, error
					{
						_error(12, symbles[iter].getLineNumber());
					}
				}
				else	//) not found, error
				{
					_error(11, symbles[iter].getLineNumber());
				}
			}
			else	//( not found, error
			{
				_error(2, symbles[iter].getLineNumber());
			}
		}
		else	//identifier not found, error
		{
			_error(1, symbles[iter].getLineNumber());
		}

	}
	else	//void not found, error
	{
		_error(14, symbles[iter].getLineNumber());
	}

	printInfo("This is a ＜无返回值函数定义＞");
	return true;
}

//＜主函数＞    ::= void main‘(’‘)’ ‘{’＜复合语句＞‘}’
bool SyntaxAnalyzer::mainFunction()
{
	string name = "main";
	string type = "void";
	string kind = "main_function";
	TableItem tableItem;
	int value = 0;
	//void
	if (symbles[iter].getType() == "PRESERVED_WORD_VOID")
	{
		iter++;
		//main
		if (symbles[iter].getType() == "PRESERVED_WORD_MAIN")
		{
			funcName = symbles[iter].getValue();
			table.newTable(name);
			iter++;
			//‘(’‘)’
			if (symbles[iter].getType() == "LPAREN")		//(
			{
				iter++;
				if (symbles[iter].getType() == "RPAREN")	//)
				{
					tableItem = TableItem(name, kind, type);
					tableItem.setValue(value);
					table.put("globle", name, tableItem);
					iter++;
					if (symbles[iter].getType() == "LBRACE")	//{
					{
						iter++;

						imCodeGenerator.genProc(name);
						compondStatement();
						imCodeGenerator.genEndp(name);

						if (symbles[iter].getType() == "RBRACE")	//}
						{
							iter++;
						}
						else	//} not found, error
						{
							_error(13, symbles[iter].getLineNumber());
						}
					}
					else	//{ not found, error
					{
						_error(12, symbles[iter].getLineNumber());
					}
				}
				else	//) not found, error
				{
					_error(11, symbles[iter].getLineNumber());
				}
			}
			else	//( not found, error
			{
				_error(2, symbles[iter].getLineNumber());
			}
		}
		else	//main not found, error
		{
			_error(15, symbles[iter].getLineNumber());
		}

	}
	else	//void not found, error
	{
		_error(14, symbles[iter].getLineNumber());
	}

	printInfo("This is a ＜主函数＞");
	return true;
}

//＜参数表＞    ::=  ＜类型标识符＞＜标识符＞{,＜类型标识符＞＜标识符＞}| ＜空＞
int SyntaxAnalyzer::parameter()
{
	int paraCount = 0;
	string name;
	string type;
	string kind = "parameter";
	TableItem tableItem;
	int value = 0;

	if (symbles[iter].getType() == "PRESERVED_WORD_INT" ||
		symbles[iter].getType() == "PRESERVED_WORD_CHAR")
	{
		paraCount++;
		value++;
		type = typeIdentifier();
		if (symbles[iter].getType() == "IDENTIFIER")
		{
			name = symbles[iter].getValue();
			tableItem = TableItem(name, kind, type);
			tableItem.setValue(value);
			table.put(funcName, name, tableItem);
			iter++;
			while (symbles[iter].getType() == "COMMA")	//{,＜类型标识符＞＜标识符＞}
			{
				iter++;
				paraCount++;
				value++;
				type = typeIdentifier();
				if (symbles[iter].getType() == "IDENTIFIER")
				{
					name = symbles[iter].getValue();
					tableItem = TableItem(name, kind, type);
					tableItem.setValue(value);
					table.put(funcName, name, tableItem);
					iter++;
				}
				else	//identifier not found, error
				{
					_error(1, symbles[iter].getLineNumber());
				}
			}
		}
		else	//identifier not found, error
		{
			_error(1, symbles[iter].getLineNumber());
		}

	}

	printInfo("This is a ＜参数表＞");
	return paraCount;
}

//＜复合语句＞   ::=  ［＜常量说明＞］［＜变量说明＞］＜语句列＞
bool SyntaxAnalyzer::compondStatement()
{
	//［＜常量说明＞］
	if (symbles[iter].getType() == "PRESERVED_WORD_CONST")		//const
		constExplanation();

	//［＜变量说明＞］or ＜有返回值函数定义＞
	if (symbles[iter].getType() == "PRESERVED_WORD_INT" ||		//int
		symbles[iter].getType() == "PRESERVED_WORD_CHAR")		//char
		varExplanation();

	statementList();

	printInfo("This is a ＜复合语句＞");
	return true;
}

//＜语句列＞   ::=｛＜语句＞｝
bool SyntaxAnalyzer::statementList()
{
	while (symbles[iter].getType() == "PRESERVED_WORD_IF" ||
		symbles[iter].getType() == "PRESERVED_WORD_FOR" ||
		symbles[iter].getType() == "PRESERVED_WORD_DO" ||
		symbles[iter].getType() == "LBRACE" ||
		symbles[iter].getType() == "IDENTIFIER" ||
		symbles[iter].getType() == "PRESERVED_WORD_PRINTF" ||
		symbles[iter].getType() == "PRESERVED_WORD_SCANF" ||
		symbles[iter].getType() == "PRESERVED_WORD_RETURN" ||
		symbles[iter].getType() == "SEMICOLON")
		statement();

	printInfo("This is a ＜语句列＞");
	return true;
}

//＜语句＞    ::= ＜条件语句＞｜＜循环语句＞｜‘{’＜语句列＞‘}’｜＜有返回值函数调用语句＞; | ＜无返回值函数调用语句＞; ｜＜赋值语句＞; ｜＜读语句＞; ｜＜写语句＞; ｜＜空＞; ｜＜返回语句＞;
bool SyntaxAnalyzer::statement()
{
	if (symbles[iter].getType() == "PRESERVED_WORD_IF")			//＜条件语句＞
	{
		ifStatement();
	}
	else if (symbles[iter].getType() == "PRESERVED_WORD_FOR")	//for
	{
		forStatement();
	}
	else if (symbles[iter].getType() == "PRESERVED_WORD_DO")	//do while
	{
		doWhileStatement();
	}
	else if (symbles[iter].getType() == "LBRACE")				//‘{’＜语句列＞‘}’
	{
		iter++;
		statementList();
		if (symbles[iter].getType() == "RBRACE")
		{
			iter++;
		}
		else	//} not found, error
		{
			_error(13, symbles[iter].getLineNumber());
		}
	}
	else if (symbles[iter].getType() == "IDENTIFIER")			//＜有返回值函数调用语句＞; | ＜无返回值函数调用语句＞; |＜赋值语句＞;
	{
		iter++;
		if (symbles[iter].getType() == "LPAREN")	//＜有返回值函数调用语句＞; | ＜无返回值函数调用语句＞;
		{
			iter--;
			returnFunctionCall();
			if (symbles[iter].getType() == "SEMICOLON")
			{
				iter++;
			}
			else	//; not found, error
			{
				_error(4, symbles[iter].getLineNumber());
			}
		}
		else //＜赋值语句＞;
		{
			iter--;
			assignStatement();
			if (symbles[iter].getType() == "SEMICOLON")
			{
				iter++;
			}
			else	//; not found, error
			{
				_error(4, symbles[iter].getLineNumber());
			}
		}
	}
	else if (symbles[iter].getType() == "PRESERVED_WORD_PRINTF")//＜写语句＞;
	{
		printfStatement();
		if (symbles[iter].getType() == "SEMICOLON")
		{
			iter++;
		}
		else	//; not found, error
		{
			_error(4, symbles[iter].getLineNumber());
		}

	}
	else if (symbles[iter].getType() == "PRESERVED_WORD_SCANF")	//＜读语句＞;
	{
		scanfStatement();
		if (symbles[iter].getType() == "SEMICOLON")
		{
			iter++;
		}
		else	//; not found, error
		{
			_error(4, symbles[iter].getLineNumber());
		}
	}
	else if (symbles[iter].getType() == "PRESERVED_WORD_RETURN")//＜返回语句＞;
	{
		returnStatement();

		if (symbles[iter].getType() == "SEMICOLON")
		{
			iter++;
		}
		else	//; not found, error
		{
			_error(4, symbles[iter].getLineNumber());
		}
	}
	else if (symbles[iter].getType() == "SEMICOLON")			//＜空＞; 
	{
		iter++;
	}
	else	//illegal statement, error
	{
		_error(25, symbles[iter].getLineNumber());
	}

	printInfo("This is a <语句>");
	return true;
}

//＜条件语句＞  ::=  if ‘(’＜条件＞‘)’＜语句＞［else＜语句＞］
bool SyntaxAnalyzer::ifStatement()
{
	int flag = 0;	//whether there is an "else"
	string label1;	//the label before "else", if the condition is false, jmp to label1 from the begin of this ifStatement
	string label2;	//the label at the end of this ifStatement (if there is a "else"), jmp to label2 from the previous statement of "else" if the condition is true
	string tempName;
	label1 = labelManager.getLabel();

	//if
	if (symbles[iter].getType() == "PRESERVED_WORD_IF")
		iter++;
	else	//if not found, error
	{
		_error(16, symbles[iter].getLineNumber());
	}

	//(
	if (symbles[iter].getType() == "LPAREN")
		iter++;
	else	//if not found, error
	{
		_error(2, symbles[iter].getLineNumber());
	}

	tempName = condition();
	imCodeGenerator.genJz(tempName, label1);	//jz: condition==0 then jump to label1

	//)
	if (symbles[iter].getType() == "RPAREN")
		iter++;
	else	//if not found, error
	{
		_error(11, symbles[iter].getLineNumber());
	}

	statement();

	//［else＜语句＞］
	if (symbles[iter].getType() == "PRESERVED_WORD_ELSE")
	{
		label2 = labelManager.getLabel();
		imCodeGenerator.genJmp(label2);
		imCodeGenerator.genLabel(label1);	//generate label1, if the condition is false, jump to here
		iter++;
		statement();
		imCodeGenerator.genLabel(label2);	//generate label2, if the condition is true, jump to here from before "else" to skip those "else statements"
	}
	else	//there is no "else" after "if"
	{
		imCodeGenerator.genLabel(label1);	//generate label1, if the condition is false, jump to here
	}


	printInfo("This is a ＜条件语句＞");
	return true;
}

//＜条件＞    ::=  ＜表达式＞＜关系运算符＞＜表达式＞｜＜表达式＞ //表达式为0条件为假，否则为真
string SyntaxAnalyzer::condition()
{

	string tempName;
	string arg1;
	string arg2;
	string op;

	arg1 = expression();
	if (symbles[iter].getType() == "EQUAL" ||
		symbles[iter].getType() == "UNEQUAL" ||
		symbles[iter].getType() == "LESS" ||
		symbles[iter].getType() == "LESS_OR_EQUAL" ||
		symbles[iter].getType() == "GREATER" ||
		symbles[iter].getType() == "GREATER_OR_EQUAL")
	{
		op = symbles[iter].getValue();
		iter++;
		arg2 = expression();
		tempName = tmpNameManager.getTempName();
		table.put(funcName, tempName, TableItem(tempName, "temp", "int"));
		imCodeGenerator.gen4(op, arg1, arg2, tempName);
		arg1 = tempName;
	}

	printInfo("This is a <条件>");
	return arg1;
}

//＜表达式＞    ::= ［＋｜－］＜项＞{＜加法运算符＞＜项＞}
string SyntaxAnalyzer::expression()
{
	string tempName;
	string arg1;
	string arg2;
	string op;
	int flag = 0;

	if (symbles[iter].getType() == "PLUS" ||
		symbles[iter].getType() == "MINUS")
	{
		if (symbles[iter].getType() == "MINUS")
			flag = 1;
		iter++;
	}
	arg1 = term();
	if (flag == 1)
	{
		tempName = tmpNameManager.getTempName();
		table.put(funcName, tempName, TableItem(tempName, "temp", "int"));
		imCodeGenerator.gen4("-", "0", arg1, tempName);
		arg1 = tempName;
	}
	while (symbles[iter].getType() == "PLUS" ||
		symbles[iter].getType() == "MINUS")
	{
		op = symbles[iter].getValue();
		iter++;
		arg2 = term();
		tempName = tmpNameManager.getTempName();
		table.put(funcName, tempName, TableItem(tempName, "temp", "int"));
		imCodeGenerator.gen4(op, arg1, arg2, tempName);
		arg1 = tempName;
	}

	printInfo("This is a <表达式>");
	return arg1;
}


//＜项＞     ::= ＜因子＞{＜乘法运算符＞＜因子＞}
string SyntaxAnalyzer::term()
{
	string tempName;
	string arg1;
	string arg2;
	string op;
	arg1 = factor();
	while (symbles[iter].getType() == "TIMES" ||
		symbles[iter].getType() == "SLASH")
	{
		op = symbles[iter].getValue();
		iter++;
		arg2 = factor();
		tempName = tmpNameManager.getTempName();
		table.put(funcName, tempName, TableItem(tempName, "temp", "int"));
		imCodeGenerator.gen4(op, arg1, arg2, tempName);
		arg1 = tempName;
	}

	printInfo("This is a <项>");
	return arg1;
}

//＜因子＞    ::= ＜标识符＞｜＜标识符＞‘[’＜表达式＞‘]’｜＜整数＞|＜字符＞｜＜有返回值函数调用语句＞|‘(’＜表达式＞‘)’
string SyntaxAnalyzer::factor()
{
	string temp;
	string kind;
	string type;
	string name;
	TableItem tableItem;

	//标识符
	if (symbles[iter].getType() == "IDENTIFIER")
	{
		name = symbles[iter].getValue();
		if (table.check(funcName, name))	//id is in table[function]
		{
			tableItem = table.get(funcName, name);
		}
		else if (table.check("globle", name))
		{
			tableItem = table.get("globle", name);
		}
		else	//identifier not found, error
		{
			_error(27, symbles[iter].getLineNumber());
		}

		iter++;
		if (symbles[iter].getType() == "LBRACKET")	//＜标识符＞‘[’＜表达式＞‘]’
		{
			iter++;

			if (tableItem.getKind() == "array")
			{
				temp = name;
			}
			else	//identifier has a wrong kind, error
			{
				_error(28, symbles[iter - 1].getLineNumber());
			}
			temp += '[';
			temp += expression();
			temp += ']';
			if (symbles[iter].getType() == "RBRACKET")
			{
				iter++;
			}
			else	//] not found, error
			{
				_error(8, symbles[iter].getLineNumber());
			}
		}
		else if (symbles[iter].getType() == "LPAREN")	//＜有返回值函数调用语句＞
		{
			if (table.check("globle", name))
			{
				tableItem = table.get("globle", name);
				if (tableItem.getKind() == "return_function")
				{
				}
				else	//identifier has a wrong kind, error
				{
					_error(28, symbles[iter - 1].getLineNumber());
				}
			}
			else	//identifier has a wrong kind, error
			{
				_error(28, symbles[iter - 1].getLineNumber());
			}
			iter--;
			temp = returnFunctionCall();
		}
		else	//＜标识符＞
		{
			if (tableItem.getKind() == "variable" ||
				tableItem.getKind() == "const" ||
				tableItem.getKind() == "parameter")
			{
				temp = name;
			}
			else	//identifier has a wrong kind, error
			{
				_error(28, symbles[iter - 1].getLineNumber());
			}

		}
	}
	//整数
	else if (symbles[iter].getType() == "PLUS" ||
		symbles[iter].getType() == "MINUS" ||
		symbles[iter].getType() == "UNSIGNEDINT" ||
		symbles[iter].getType() == "ZERO")
	{
		temp = util::int2string(integer());
	}
	//＜字符＞
	else if (symbles[iter].getType() == "CHAR")
	{
		temp = util::int2string((int)_char());
	}
	//‘(’＜表达式＞‘)’
	else if (symbles[iter].getType() == "LPAREN")
	{
		iter++;
		temp = expression();
		if (symbles[iter].getType() == "RPAREN")
		{
			iter++;
		}
		else	//) not found, error
		{
			_error(11, symbles[iter].getLineNumber());
		}
	}
	else	//illegal factor
	{
		_error(17, symbles[iter].getLineNumber());
	}

	printInfo("This is a <因子>");
	return temp;

}

//＜有返回值函数调用语句＞ :: = ＜标识符＞‘(’＜值参数表＞‘)’
string SyntaxAnalyzer::returnFunctionCall()
{
	int flag = 0;
	string temp = "";
	string name;
	TableItem tableItem;

	if (symbles[iter].getType() == "IDENTIFIER")
	{
		name = symbles[iter].getValue();
		if (table.check("globle", name))
		{
			tableItem = table.get("globle", name);
		}
		else	//identifier not found, error
		{
			_error(27, symbles[iter].getLineNumber());
		}

		if (tableItem.getKind() == "return_function" ||
			tableItem.getKind() == "void_function")
		{
			if (tableItem.getKind() == "return_function")
				flag = 1;
		}
		else	//identifier has a wrong kind, error
		{
			_error(28, symbles[iter].getLineNumber());
		}
		iter++;
	}
	else	//identifier not found, error
	{
		_error(1, symbles[iter].getLineNumber());
	}

	if (symbles[iter].getType() == "LPAREN")
	{
		iter++;
	}
	else	//( not found, error
	{
		_error(2, symbles[iter].getLineNumber());
	}

	parameterList(tableItem.getValue());
	if (flag == 1)
	{
		temp = tmpNameManager.getTempName();
		table.put(funcName, temp, TableItem(temp, "temp", tableItem.getType()));
		imCodeGenerator.genCall(name, temp);
	}
	else
		imCodeGenerator.genVoidCall(name);

	if (symbles[iter].getType() == "RPAREN")
	{
		iter++;
	}
	else	//) not found, error
	{
		_error(11, symbles[iter].getLineNumber());
	}

	printInfo("This is a <有返回值函数调用>");
	return temp;
}

//这个没有卵用啊
//＜无返回值函数调用语句＞ ::= ＜标识符＞‘(’＜值参数表＞‘)’
/*
bool SyntaxAnalyzer::voidFunctionCall()
{
	if (symbles[iter].getType() == "IDENTIFIER")
	{
		iter++;
	}
	else	//identifier not found, error
	{
		_error(1, symbles[iter].getLineNumber());
	}

	if (symbles[iter].getType() == "LPAREN")
	{
		iter++;
	}
	else	//( not found, error
	{
		_error(2, symbles[iter].getLineNumber());
	}

	parameterList(1);

	if (symbles[iter].getType() == "RPAREN")
	{
		iter++;
	}
	else	//) not found, error
	{
		_error(11, symbles[iter].getLineNumber());
	}

	printInfo("This is a <无返回值函数调用>");
	return true;
}
*/


//＜值参数表＞   :: = ＜表达式＞{ ,＜表达式＞ }｜＜空＞
bool SyntaxAnalyzer::parameterList(int count)
{
	int c = 0;	//parameter count
	string temp;
	//＜空＞
	if (symbles[iter].getType() == "RPAREN");
	else
	{
		temp = expression();
		c++;
		imCodeGenerator.genPushpara(temp);
		while (symbles[iter].getType() == "COMMA")
		{
			iter++;
			temp = expression();
			c++;
			imCodeGenerator.genPushpara(temp);
		}
	}

	if (c < count)	//not enough parameters, error
	{
		_error(30, symbles[iter].getLineNumber());
	}
	else if (c>count)	//too many parameters, error
	{
		_error(29, symbles[iter].getLineNumber());
	}

	printInfo("This is a <值参数表>");
	return true;
}

//for‘(’＜标识符＞＝＜表达式＞;＜条件＞;＜标识符＞＝＜标识符＞(+|-)＜步长＞‘)’＜语句＞
bool SyntaxAnalyzer::forStatement()
{
	//the structure is like this:
	/*
		i=0;
		L2
		jz condition L1
		{statements}
		i++;
		jmp L2;
		L1
	*/
	string label1;
	string label2;
	string tempName;
	string name;			//i
	TableItem tableItem;	//i
	string name2;			//for(;;j=k+1) j
	string name3;			//for(;;j=k+1) k
	string op;				//for(;;j=k+1) +
	string step;			//for(;;j=k+1) 1

	label1 = labelManager.getLabel();
	label2 = labelManager.getLabel();

	if (symbles[iter].getType() == "PRESERVED_WORD_FOR")
	{
		iter++;
	}
	else	//for not found, error
	{
		_error(18, symbles[iter].getLineNumber());
	}

	if (symbles[iter].getType() == "LPAREN")
	{
		iter++;
	}
	else	//( not found, error
	{
		_error(2, symbles[iter].getLineNumber());
	}

	if (symbles[iter].getType() == "IDENTIFIER")
	{
		name = symbles[iter].getValue();
		if (table.check(funcName, name))	//id is in table[function]
		{
			tableItem = table.get(funcName, name);
		}
		else if (table.check("globle", name))
		{
			tableItem = table.get("globle", name);
		}
		else	//identifier not found, error
		{
			_error(27, symbles[iter].getLineNumber());
		}

		if (tableItem.getKind() == "variable" ||
			tableItem.getKind() == "parameter")
		{
		}
		else	//identifier has a wrong kind, error
		{
			_error(28, symbles[iter].getLineNumber());
		}

		iter++;
	}
	else	//identifier not found, error
	{
		_error(1, symbles[iter].getLineNumber());
	}

	if (symbles[iter].getType() == "ASSIGN")
	{
		iter++;
	}
	else	//identifier not found, error
	{
		_error(6, symbles[iter].getLineNumber());
	}

	tempName = expression();
	imCodeGenerator.genAssign(tempName, name);
	imCodeGenerator.genLabel(label2);

	if (symbles[iter].getType() == "SEMICOLON")
	{
		iter++;
	}
	else	//; not found, error
	{
		_error(4, symbles[iter].getLineNumber());
	}

	tempName = condition();
	imCodeGenerator.genJz(tempName, label1);

	if (symbles[iter].getType() == "SEMICOLON")
	{
		iter++;
	}
	else	//; not found, error
	{
		_error(4, symbles[iter].getLineNumber());
	}

	if (symbles[iter].getType() == "IDENTIFIER")
	{
		name2 = symbles[iter].getValue();
		if (table.check(funcName, name2))	//id is in table[function]
		{
			tableItem = table.get(funcName, name2);
		}
		else if (table.check("globle", name2))
		{
			tableItem = table.get("globle", name2);
		}
		else	//identifier not found, error
		{
			_error(27, symbles[iter].getLineNumber());
		}

		if (tableItem.getKind() == "variable" ||
			tableItem.getKind() == "parameter")
		{
		}
		else	//identifier has a wrong kind, error
		{
			_error(28, symbles[iter].getLineNumber());
		}

		iter++;
	}
	else	//identifier not found, error
	{
		_error(1, symbles[iter].getLineNumber());
	}

	if (symbles[iter].getType() == "ASSIGN")
	{
		iter++;
	}
	else	//identifier not found, error
	{
		_error(6, symbles[iter].getLineNumber());
	}

	if (symbles[iter].getType() == "IDENTIFIER")
	{
		name3 = symbles[iter].getValue();
		if (table.check(funcName, name3))	//id is in table[function]
		{
			tableItem = table.get(funcName, name3);
		}
		else if (table.check("globle", name3))
		{
			tableItem = table.get("globle", name3);
		}
		else	//identifier not found, error
		{
			_error(27, symbles[iter].getLineNumber());
		}

		if (tableItem.getKind() == "variable" ||
			tableItem.getKind() == "parameter")
		{
		}
		else	//identifier has a wrong kind, error
		{
			_error(28, symbles[iter].getLineNumber());
		}

		iter++;
	}
	else	//identifier not found, error
	{
		_error(1, symbles[iter].getLineNumber());
	}

	if (symbles[iter].getType() == "PLUS")
	{
		op = "+";
		iter++;
	}
	else if (symbles[iter].getType() == "MINUS")
	{
		op = "-";
		iter++;
	}
	else	//+,- not found, error
	{
		_error(19, symbles[iter].getLineNumber());
	}

	if (symbles[iter].getType() == "UNSIGNEDINT")
	{
		step = symbles[iter].getValue();
		iter++;
	}
	else	//unsigned int not found, error
	{
		_error(7, symbles[iter].getLineNumber());
	}

	if (symbles[iter].getType() == "RPAREN")
	{
		iter++;
	}
	else	//identifier not found, error
	{
		_error(11, symbles[iter].getLineNumber());
	}

	statement();

	imCodeGenerator.gen4(op, name3, step, name2);
	imCodeGenerator.genJmp(label2);
	imCodeGenerator.genLabel(label1);

	printInfo("This is a <for循环语句>");
	return true;

}

//do＜语句＞while ‘(’＜条件＞‘)’
bool SyntaxAnalyzer::doWhileStatement()
{
	//the structure is like this:
	/*
	L1
	{statements}
	jnz condition L1
	*/

	string label;
	string tempName;

	label = labelManager.getLabel();
	imCodeGenerator.genLabel(label);

	if (symbles[iter].getType() == "PRESERVED_WORD_DO")
	{
		iter++;
	}
	else	//do not found, error
	{
		_error(20, symbles[iter].getLineNumber());
	}

	statement();

	if (symbles[iter].getType() == "PRESERVED_WORD_WHILE")
	{
		iter++;
	}
	else	//while not found, error
	{
		_error(21, symbles[iter].getLineNumber());
	}

	if (symbles[iter].getType() == "LPAREN")
	{
		iter++;
	}
	else	//( not found, error
	{
		_error(2, symbles[iter].getLineNumber());
	}

	tempName = condition();
	imCodeGenerator.genJnz(tempName, label);


	if (symbles[iter].getType() == "RPAREN")
	{
		iter++;
	}
	else	//while not found, error
	{
		_error(11, symbles[iter].getLineNumber());
	}

	printInfo("This is a <do while循环语句>");
	return true;

}

//＜赋值语句＞   ::=  ＜标识符＞＝＜表达式＞|＜标识符＞‘[’＜表达式＞‘]’=＜表达式＞
bool SyntaxAnalyzer::assignStatement()
{
	string temp;
	string name;
	string arg1;
	string arg2;
	TableItem tableItem;

	if (symbles[iter].getType() == "IDENTIFIER")
	{
		name = symbles[iter].getValue();
		if (table.check(funcName, name))	//id is in table[function]
		{
			tableItem = table.get(funcName, name);
		}
		else if (table.check("globle", name))
		{
			tableItem = table.get("globle", name);
		}
		else	//identifier not found, error
		{
			_error(27, symbles[iter].getLineNumber());
		}
		iter++;
	}
	else	//identifier not found, error
	{
		_error(1, symbles[iter].getLineNumber());
	}

	//‘[’＜表达式＞‘]’
	if (symbles[iter].getType() == "LBRACKET")	//array
	{
		if (tableItem.getKind() == "array")
		{
			temp = name;
		}
		else	//identifier has a wrong kind, error
		{
			_error(28, symbles[iter - 1].getLineNumber());
		}

		iter++;
		temp += '[';
		temp += expression();
		temp += ']';
		if (symbles[iter].getType() == "RBRACKET")
		{
			iter++;
		}
		else	//identifier not found, error
		{
			_error(8, symbles[iter].getLineNumber());
		}
	}
	else	//not array
	{
		if (tableItem.getKind() == "variable" ||
			tableItem.getKind() == "parameter")
		{
			temp = name;
		}
		else	//identifier has a wrong kind, error
		{
			_error(28, symbles[iter - 1].getLineNumber());
		}
	}

	if (symbles[iter].getType() == "ASSIGN")
	{
		iter++;
	}
	else	//identifier not found, error
	{
		_error(6, symbles[iter].getLineNumber());
	}

	arg1 = temp;
	arg2 = expression();
	imCodeGenerator.genAssign(arg1, arg2);

	printInfo("This is a <赋值语句>");
	return true;
}

//＜写语句＞    ::=  printf‘(’＜字符串＞,＜表达式＞‘)’|printf ‘(’＜字符串＞‘)’|printf ‘(’＜表达式＞‘)’
bool SyntaxAnalyzer::printfStatement()
{
	string temp;
	if (symbles[iter].getType() == "PRESERVED_WORD_PRINTF")
	{
		iter++;
	}
	else 	//printf not found, error
	{
		_error(23, symbles[iter].getLineNumber());
	}

	if (symbles[iter].getType() == "LPAREN")
	{
		iter++;
	}
	else	//( not found, error
	{
		_error(2, symbles[iter].getLineNumber());
	}

	if (symbles[iter].getType() == "STRING")
	{
		imCodeGenerator.genPrintStr(symbles[iter].getValue());
		iter++;
		if (symbles[iter].getType() == "COMMA")
		{
			iter++;
			temp = expression();
			imCodeGenerator.genPrintf(temp);
		}
	}
	else
	{
		temp = expression();
		imCodeGenerator.genPrintf(temp);
	}

	if (symbles[iter].getType() == "RPAREN")
	{
		iter++;
	}
	else	//) not found, error
	{
		_error(11, symbles[iter].getLineNumber());
	}

	printInfo("This is a ＜写语句＞");
	return true;
}

//＜读语句＞    :: = scanf ‘(’＜标识符＞{ ,＜标识符＞ }‘)’
bool SyntaxAnalyzer::scanfStatement()
{
	string name;
	TableItem tableItem;

	if (symbles[iter].getType() == "PRESERVED_WORD_SCANF")
	{
		iter++;
	}
	else	//scanf not found, error
	{
		_error(22, symbles[iter].getLineNumber());
	}

	if (symbles[iter].getType() == "LPAREN")
	{
		iter++;
	}
	else	//( not found, error
	{
		_error(2, symbles[iter].getLineNumber());
	}

	if (symbles[iter].getType() == "IDENTIFIER")
	{
		name = symbles[iter].getValue();
		if (table.check(funcName, name))	//id is in table[function]
		{
			tableItem = table.get(funcName, name);
		}
		else if (table.check("globle", name))
		{
			tableItem = table.get("globle", name);
		}
		else	//identifier not found, error
		{
			_error(27, symbles[iter].getLineNumber());
		}
		if (tableItem.getKind() == "variable" ||
			tableItem.getKind() == "parameter")
		{
		}
		else	//identifier has a wrong kind, error
		{
			_error(28, symbles[iter].getLineNumber());
		}
		imCodeGenerator.genScanf(name);

		iter++;
	}
	else	//identifier not found, error
	{
		_error(1, symbles[iter].getLineNumber());
	}

	while (symbles[iter].getType() == "COMMA")
	{
		iter++;
		if (symbles[iter].getType() == "IDENTIFIER")
		{
			name = symbles[iter].getValue();
			if (table.check(funcName, name))	//id is in table[function]
			{
				tableItem = table.get(funcName, name);
			}
			else if (table.check("globle", name))
			{
				tableItem = table.get("globle", name);
			}
			else	//identifier not found, error
			{
				_error(27, symbles[iter].getLineNumber());
			}
			if (tableItem.getKind() == "variable" ||
				tableItem.getKind() == "parameter")
			{
			}
			else	//identifier has a wrong kind, error
			{
				_error(28, symbles[iter].getLineNumber());
			}
			imCodeGenerator.genScanf(name);

			iter++;
		}
		else	//identifier not found, error
		{
			_error(1, symbles[iter].getLineNumber());
		}
	}

	if (symbles[iter].getType() == "RPAREN")
	{
		iter++;
	}
	else	//) not found, error
	{
		_error(11, symbles[iter].getLineNumber());
	}

	printInfo("This is a <读语句>");
	return true;
}

//＜返回语句＞   ::=  return[‘(’＜表达式＞‘)’]
bool SyntaxAnalyzer::returnStatement()
{
	string temp;
	if (symbles[iter].getType() == "PRESERVED_WORD_RETURN")
	{
		iter++;
	}
	else	//return not found, error
	{
		_error(24, symbles[iter].getLineNumber());
	}

	if (symbles[iter].getType() == "LPAREN")
	{
		iter++;
		temp = expression();
		imCodeGenerator.genReturn(temp);

		if (symbles[iter].getType() == "RPAREN")
		{
			iter++;
		}
		else	//) not found, error
		{
			_error(11, symbles[iter].getLineNumber());
		}

	}

	printInfo("This is a <返回语句>");
	return true;
}


void SyntaxAnalyzer::_error(int errorType, int lineNumber)
{
	syntaxErrors.push_back(Error(errorType, lineNumber));
	//iter++;
}

Error::Error(int errorType, int lineNumber)
{
	this->lineNumber = lineNumber;
	this->type = errorType;
}

void Error::print()
{

	//1 identifier
	//2 (
	//3 const
	//4 ;
	//5 type identifier(int|char)
	//6 =
	//7 unsigned int
	//8 ]
	//9 integer
	//10 char
	//11 )
	//12 {
	//13 }
	//14 void
	//15 main
	//16 if
	//17 illegal factor
	//18 for
	//19 + -
	//20 do
	//21 while
	//22 scanf
	//23 printf
	//24 return 
	//25 illegal statement
	//26 identifier redeclaration
	//27 undefined identifier
	//28 identifier has a wrong kind
	//29 too many parameters
	//30 not enough parameters

	cout << "[line" << this->lineNumber << "]\t";
	switch (this->type)
	{
	case 1:
		cout << "missing identifier\n";
		break;
	case 2:
		cout << "missing '('\n";
		break;
	case 3:
		cout << "missing \"const\"\n";
		break;
	case 4:
		cout << "missing ';'\n";
		break;
	case 5:
		cout << "missing type identifier(\"int\" or \"char\")\n";
		break;
	case 6:
		cout << "missing '='\n";
		break;
	case 7:
		cout << "missing unsigned integer\n";
		break;
	case 8:
		cout << "missing ']'\n";
		break;
	case 9:
		cout << "missing integer\n";
		break;
	case 10:
		cout << "missing char\n";
		break;
	case 11:
		cout << "missing ')'\n";
		break;
	case 12:
		cout << "missing '{'\n";
		break;
	case 13:
		cout << "missing '}'\n";
		break;
	case 14:
		cout << "missing \"void\"\n";
		break;
	case 15:
		cout << "missing \"main\"\n";
		break;
	case 16:
		cout << "missing \"if\"\n";
		break;
	case 17:
		cout << "illegal factor\n";
		break;
	case 18:
		cout << "missing \"for\"\n";
		break;
	case 19:
		cout << "missing '+' or '-'\n";
		break;
	case 20:
		cout << "missing \"do\"\n";
		break;
	case 21:
		cout << "missing \"while\"\n";
		break;
	case 22:
		cout << "missing \"scanf\"\n";
		break;
	case 23:
		cout << "missing \"printf\"\n";
		break;
	case 24:
		cout << "missing \"return\"\n";
		break;
	case 25:
		cout << "illegal statement\n";
		break;
	case 26:
		cout << "identifier redeclaration\n";
		break;
	case 27:
		cout << "undeclared identifier\n";
		break;
	case 28:
		cout << "identifier has a wrong kind\n";
		break;
	case 29:
		cout << "too many parameters\n";
		break;
	case 30:
		cout << "not enough parameters\n";
		break;
	default:
		break;
	}
}

void SyntaxAnalyzer::printErrors()
{
	for (int i = 0; i < syntaxErrors.size(); i++)
		syntaxErrors[i].print();
}

Table SyntaxAnalyzer::getTable()
{
	return table;
}