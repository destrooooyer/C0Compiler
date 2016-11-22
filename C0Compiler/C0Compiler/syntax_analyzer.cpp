#include "syntax_analyzer.h"
#include "lexical_analyzer.h"
#include <iostream>
#include <vector>

using namespace std;

SyntaxAnalyzer::SyntaxAnalyzer(vector<Symble> symbles)
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


//������    :: = �ۣ�����˵�����ݣۣ�����˵������{ ���з���ֵ�������壾 | ���޷���ֵ�������壾 }����������
bool SyntaxAnalyzer::program()
{
	funcName = "globle";
	//�ۣ�����˵������
	if (symbles[iter].getType() == "PRESERVED_WORD_CONST")		//const
		constExplanation();

	//�ۣ�����˵������or ���з���ֵ�������壾
	if (symbles[iter].getType() == "PRESERVED_WORD_INT" ||		//int
		symbles[iter].getType() == "PRESERVED_WORD_CHAR")		//char
	{
		int iterBackUp = iter;	//create a back up for iter

		iter++;
		if (symbles[iter].getType() == "IDENTIFIER")
		{
			iter++;
			if (symbles[iter].getType() == "LPAREN")	//it should be ���з���ֵ�������壾
			{
				iter = iterBackUp;	//recover iter;
				returnFunctionDeclaration();
			}
			else	//it should be ������˵����
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

	// 	//���޷���ֵ�������壾 or ����������
	// 	if (symbles[iter].getType() == "PRESERVED_WORD_VOID")		//void
	// 	{
	// 		int iterBackUp = iter;	//create a back up for iter
	// 
	// 		iter++;
	// 		if (symbles[iter].getType() == "PRESERVED_WORD_MAIN")
	// 		{
	// 			iter = iterBackUp;
	// 			mainFunction();
	// 			printInfo("This is a ������");
	// 			return true;
	// 		}
	// 		else //void function
	// 		{
	// 			iter = iterBackUp;
	// 			voidFunctionDeclaration();
	// 		}
	// 	}


		//{ ���з���ֵ�������壾 | ���޷���ֵ�������壾 }| ����������
	while (symbles[iter].getType() == "PRESERVED_WORD_INT" ||	//int
		symbles[iter].getType() == "PRESERVED_WORD_CHAR" ||		//char
		symbles[iter].getType() == "PRESERVED_WORD_VOID")		//void
	{
		//���޷���ֵ�������壾 or ����������
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
		//���з���ֵ�������壾
		else if (symbles[iter].getType() == "PRESERVED_WORD_INT" ||
			symbles[iter].getType() == "PRESERVED_WORD_CHAR")
		{
			returnFunctionDeclaration();
		}
	}

	printInfo("This is a ������");
	return true;
}

//������˵���� ::=  const���������壾;{ const���������壾;}
bool SyntaxAnalyzer::constExplanation()
{
	//const
	if (symbles[iter].getType() == "PRESERVED_WORD_CONST")
	{
		iter++;
		//���������壾
		constDeclaration();

		//;
		if (symbles[iter].getType() == "SEMICOLON")
		{
			iter++;
			//{ const���������壾; }
			while (symbles[iter].getType() == "PRESERVED_WORD_CONST")	//while next symble is "const"
			{
				iter++;
				//���������壾
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

	printInfo("This is a ������˵����");
	return true;
}

//���������壾   ::=   int����ʶ��������������{,����ʶ��������������} | char����ʶ���������ַ���{ ,����ʶ���������ַ��� }
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
				//��������
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
							//��������
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
				//���ַ���
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
							//���ַ���
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

	printInfo("This is a ���������壾");
	return true;
}

//��������        ::= �ۣ������ݣ��޷�������������
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
	printInfo("This is a <����>");
	return value*factor;
}

//���ַ���    ::=  '���ӷ��������'��'���˷��������'��'����ĸ��'��'�����֣�'
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

	printInfo("This is a <�ַ�>");
	return temp;
}

//������˵����  ::= ���������壾;{���������壾;}
bool SyntaxAnalyzer::varExplanation()
{
	varDeclaration();
	if (symbles[iter].getType() == "SEMICOLON")
	{
		iter++;
		//{���������壾;}
		while (true)
		{
			//���������壾or ���з���ֵ�������壾
			if (symbles[iter].getType() == "PRESERVED_WORD_INT" ||		//int
				symbles[iter].getType() == "PRESERVED_WORD_CHAR")		//char
			{
				int iterBackUp = iter;	//create a back up for iter

				iter++;
				if (symbles[iter].getType() == "IDENTIFIER")
				{
					iter++;
					if (symbles[iter].getType() != "LPAREN")	//it is not ���з���ֵ�������壾, so it is���������壾
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
					else	//it should be ���з���ֵ�������壾
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


	printInfo("This is a ������˵����");
	return true;
}

//���������壾  ::= �����ͱ�ʶ����(����ʶ����|����ʶ������[�����޷�����������]��){,(����ʶ����|����ʶ������[�����޷�����������]��) }
bool SyntaxAnalyzer::varDeclaration()
{
	string name;
	string type;
	string kind;
	TableItem tableItem;
	int value;

	//�����ͱ�ʶ����
	type = typeIdentifier();

	//����ʶ����
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
		//��[�����޷�����������]��
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

		//{,(����ʶ����|����ʶ������[�����޷�����������]��) }
		while (symbles[iter].getType() == "COMMA")
		{
			iter++;
			//����ʶ����
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

				//��[�����޷�����������]��
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


	printInfo("This is a ���������壾");
	return true;
}

//�����ͱ�ʶ����      ::=  int | char
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

	printInfo("This is a �����ͱ�ʶ����");
	return temp;
}

//���з���ֵ�������壾  ::=  ������ͷ������(������������)�� ��{����������䣾��}��
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
			iter++;
			//��(������������)��
			if (symbles[iter].getType() == "LPAREN")		//(
			{
				iter++;
				value = parameter();	//��������
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
						imCodeGenerator.genRet(name);

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

	printInfo("This is a ���з���ֵ�������壾");
	return true;
}


//���޷���ֵ�������壾  ::= void����ʶ������(������������)����{����������䣾��}��
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
			iter++;
			//��(������������)��
			if (symbles[iter].getType() == "LPAREN")		//(
			{
				iter++;
				value = parameter();	//��������
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
						imCodeGenerator.genRet(name);

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

	printInfo("This is a ���޷���ֵ�������壾");
	return true;
}

//����������    ::= void main��(����)�� ��{����������䣾��}��
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
			iter++;
			//��(����)��
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
						imCodeGenerator.genRet(name);

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

	printInfo("This is a ����������");
	return true;
}

//��������    ::=  �����ͱ�ʶ��������ʶ����{,�����ͱ�ʶ��������ʶ����}| ���գ�
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
		type = typeIdentifier();
		if (symbles[iter].getType() == "IDENTIFIER")
		{
			name = symbles[iter].getValue();
			tableItem = TableItem(name, kind, type);
			tableItem.setValue(value);
			table.put(funcName, name, tableItem);
			iter++;
			while (symbles[iter].getType() == "COMMA")	//{,�����ͱ�ʶ��������ʶ����}
			{
				iter++;
				paraCount++;
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

	printInfo("This is a ��������");
	return paraCount;
}

//��������䣾   ::=  �ۣ�����˵�����ݣۣ�����˵�����ݣ�����У�
bool SyntaxAnalyzer::compondStatement()
{
	//�ۣ�����˵������
	if (symbles[iter].getType() == "PRESERVED_WORD_CONST")		//const
		constExplanation();

	//�ۣ�����˵������or ���з���ֵ�������壾
	if (symbles[iter].getType() == "PRESERVED_WORD_INT" ||		//int
		symbles[iter].getType() == "PRESERVED_WORD_CHAR")		//char
		varExplanation();

	statementList();

	printInfo("This is a ��������䣾");
	return true;
}

//������У�   ::=������䣾��
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

	printInfo("This is a ������У�");
	return true;
}

//����䣾    ::= ��������䣾����ѭ����䣾����{��������У���}�������з���ֵ����������䣾; | ���޷���ֵ����������䣾; ������ֵ��䣾; ��������䣾; ����д��䣾; �����գ�; ����������䣾;
bool SyntaxAnalyzer::statement()
{
	if (symbles[iter].getType() == "PRESERVED_WORD_IF")			//��������䣾
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
	else if (symbles[iter].getType() == "LBRACE")				//��{��������У���}��
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
	else if (symbles[iter].getType() == "IDENTIFIER")			//���з���ֵ����������䣾; | ���޷���ֵ����������䣾; |����ֵ��䣾;
	{
		iter++;
		if (symbles[iter].getType() == "LPAREN")	//���з���ֵ����������䣾; | ���޷���ֵ����������䣾;
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
		else //����ֵ��䣾;
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
	else if (symbles[iter].getType() == "PRESERVED_WORD_PRINTF")//��д��䣾;
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
	else if (symbles[iter].getType() == "PRESERVED_WORD_SCANF")	//������䣾;
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
	else if (symbles[iter].getType() == "PRESERVED_WORD_RETURN")//��������䣾;
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
	else if (symbles[iter].getType() == "SEMICOLON")			//���գ�; 
	{
		iter++;
	}
	else	//illegal statement, error
	{
		_error(25, symbles[iter].getLineNumber());
	}

	printInfo("This is a <���>");
	return true;
}

//��������䣾  ::=  if ��(������������)������䣾��else����䣾��
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

	//��else����䣾��
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


	printInfo("This is a ��������䣾");
	return true;
}

//��������    ::=  �����ʽ������ϵ������������ʽ���������ʽ�� //���ʽΪ0����Ϊ�٣�����Ϊ��
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

	printInfo("This is a <����>");
	return arg1;
}

//�����ʽ��    ::= �ۣ������ݣ��{���ӷ�����������}
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

	printInfo("This is a <���ʽ>");
	return arg1;
}


//���     ::= �����ӣ�{���˷�������������ӣ�}
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

	printInfo("This is a <��>");
	return arg1;
}

//�����ӣ�    ::= ����ʶ����������ʶ������[�������ʽ����]������������|���ַ��������з���ֵ����������䣾|��(�������ʽ����)��
string SyntaxAnalyzer::factor()
{
	string temp;
	string kind;
	string type;
	string name;
	TableItem tableItem;

	//��ʶ��
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
			_error(27, symbles[iter - 1].getLineNumber());
		}

		iter++;
		if (symbles[iter].getType() == "LBRACKET")	//����ʶ������[�������ʽ����]��
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
		if (symbles[iter].getType() == "LPAREN")	//���з���ֵ����������䣾
		{
			iter--;
			temp = returnFunctionCall();
		}
		else	//����ʶ����
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
	//����
	else if (symbles[iter].getType() == "PLUS" ||
		symbles[iter].getType() == "MINUS" ||
		symbles[iter].getType() == "UNSIGNEDINT" ||
		symbles[iter].getType() == "ZERO")
	{
		temp = util::int2string(integer());
	}
	//���ַ���
	else if (symbles[iter].getType() == "CHAR")
	{
		temp = util::int2string((int)_char());
	}
	//��(�������ʽ����)��
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

	printInfo("This is a <����>");
	return temp;

}

//���з���ֵ����������䣾 :: = ����ʶ������(����ֵ��������)��
string SyntaxAnalyzer::returnFunctionCall()
{
	string temp;
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

	parameterList();

	if (symbles[iter].getType() == "RPAREN")
	{
		iter++;
	}
	else	//) not found, error
	{
		_error(11, symbles[iter].getLineNumber());
	}

	printInfo("This is a <�з���ֵ��������>");
	return temp;
}

//���޷���ֵ����������䣾 ::= ����ʶ������(����ֵ��������)��
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

	parameterList();

	if (symbles[iter].getType() == "RPAREN")
	{
		iter++;
	}
	else	//) not found, error
	{
		_error(11, symbles[iter].getLineNumber());
	}

	printInfo("This is a <�޷���ֵ��������>");
	return true;
}

//��ֵ������   :: = �����ʽ��{ ,�����ʽ�� }�����գ�
bool SyntaxAnalyzer::parameterList()
{
	//��
	if (symbles[iter].getType() == "RPAREN");
	else
	{
		expression();
		while (symbles[iter].getType() == "COMMA")
		{
			iter++;
			expression();
		}
	}

	printInfo("This is a <ֵ������>");
	return true;
}

//for��(������ʶ�����������ʽ��;��������;����ʶ����������ʶ����(+|-)����������)������䣾
bool SyntaxAnalyzer::forStatement()
{
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

	expression();

	if (symbles[iter].getType() == "SEMICOLON")
	{
		iter++;
	}
	else	//; not found, error
	{
		_error(4, symbles[iter].getLineNumber());
	}

	condition();

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
		iter++;
	}
	else	//identifier not found, error
	{
		_error(1, symbles[iter].getLineNumber());
	}

	if (symbles[iter].getType() == "PLUS")
	{
		iter++;
	}
	else if (symbles[iter].getType() == "MINUS")
	{
		iter++;
	}
	else	//+,- not found, error
	{
		_error(19, symbles[iter].getLineNumber());
	}

	if (symbles[iter].getType() == "UNSIGNEDINT")
	{
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

	printInfo("This is a <forѭ�����>");
	return true;

}

//do����䣾while ��(������������)��
bool SyntaxAnalyzer::doWhileStatement()
{
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

	condition();

	if (symbles[iter].getType() == "RPAREN")
	{
		iter++;
	}
	else	//while not found, error
	{
		_error(11, symbles[iter].getLineNumber());
	}

	printInfo("This is a <do whileѭ�����>");
	return true;

}

//����ֵ��䣾   ::=  ����ʶ�����������ʽ��|����ʶ������[�������ʽ����]��=�����ʽ��
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
			_error(27, symbles[iter - 1].getLineNumber());
		}
		iter++;
	}
	else	//identifier not found, error
	{
		_error(1, symbles[iter].getLineNumber());
	}

	//��[�������ʽ����]��
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

	printInfo("This is a <��ֵ���>");
	return true;
}

//��д��䣾    ::=  printf��(�����ַ�����,�����ʽ����)��|printf ��(�����ַ�������)��|printf ��(�������ʽ����)��
bool SyntaxAnalyzer::printfStatement()
{
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
		iter++;
		if (symbles[iter].getType() == "COMMA")
		{
			iter++;
			expression();
		}
	}
	else
	{
		expression();
	}

	if (symbles[iter].getType() == "RPAREN")
	{
		iter++;
	}
	else	//) not found, error
	{
		_error(11, symbles[iter].getLineNumber());
	}

	printInfo("This is a ��д��䣾");
	return true;
}

//������䣾    :: = scanf ��(������ʶ����{ ,����ʶ���� }��)��
bool SyntaxAnalyzer::scanfStatement()
{
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

	printInfo("This is a <�����>");
	return true;
}

//��������䣾   ::=  return[��(�������ʽ����)��]
bool SyntaxAnalyzer::returnStatement()
{
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
		expression();

		if (symbles[iter].getType() == "RPAREN")
		{
			iter++;
		}
		else	//) not found, error
		{
			_error(11, symbles[iter].getLineNumber());
		}

	}

	printInfo("This is a <�������>");
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