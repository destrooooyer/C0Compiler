#ifndef SYNTAX_ANALYZER
#define SYNTAX_ANALYZER

#include "lexical_analyzer.h"
#include "util.h"
#include "table.h"
#include "intermediate_code_generator.h"

#include <vector>
#include <string>

class Error
{
public:
	Error(int type, int lineNumber);
	void print();
private:
	int type;
	int lineNumber;
};

class SyntaxAnalyzer
{
public:
	SyntaxAnalyzer(std::vector<Symbol> symbles);
	bool program();						//＜程序＞ 
	bool constExplanation();			//＜常量说明＞
	bool varExplanation();				//＜变量说明＞
	bool returnFunctionDeclaration();	//＜有返回值函数定义＞
	bool voidFunctionDeclaration();		//＜无返回值函数定义＞
	bool mainFunction();				//＜主函数＞
	bool constDeclaration();			//＜常量定义＞ 
	bool varDeclaration();				//＜变量定义＞
	int integer();						//＜整数＞
	char _char();						//<字符>
	std::string typeIdentifier();		//＜类型标识符＞
	int parameter();					//<参数>
	bool compondStatement();			//<复合语句>
	bool statementList();				//<语句列>
	bool statement();					//<语句>
	bool ifStatement();					//<条件语句>
	std::string condition();			//<条件>
	std::string expression();			//<表达式>
	std::string term();					//<项>
	std::string factor();				//<因子>
	std::string returnFunctionCall();	//<有返回值函数调用语句>
	bool voidFunctionCall();			//<无返回值函数调用语句>
	bool parameterList(int count);		//<值参数表>
	bool forStatement();				//<for循环语句>
	bool doWhileStatement();			//<do while循环语句>
	bool assignStatement();				//<赋值语句>
	bool printfStatement();				//<写语句>
	bool scanfStatement();				//<读语句>
	bool returnStatement();				//<返回语句>
	Table getTable();
	std::vector<Quadruple> getIntermediateCodes();

	bool infoSwitch;

	void printErrors();

private:
	std::vector <Symbol> symbles;
	IMCodeGenerator imCodeGenerator;
	TempNameManager tmpNameManager;
	LabelManager labelManager;
	int iter;
	std::vector<Error> syntaxErrors;
	Table table;
	std::string funcName;

	void printInfo(std::string str);
	void _error(int errorType, int lineNumber);
};


#endif