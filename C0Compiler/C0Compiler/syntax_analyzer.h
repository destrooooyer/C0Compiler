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
	bool program();						//������ 
	bool constExplanation();			//������˵����
	bool varExplanation();				//������˵����
	bool returnFunctionDeclaration();	//���з���ֵ�������壾
	bool voidFunctionDeclaration();		//���޷���ֵ�������壾
	bool mainFunction();				//����������
	bool constDeclaration();			//���������壾 
	bool varDeclaration();				//���������壾
	int integer();						//��������
	char _char();						//<�ַ�>
	std::string typeIdentifier();		//�����ͱ�ʶ����
	int parameter();					//<����>
	bool compondStatement();			//<�������>
	bool statementList();				//<�����>
	bool statement();					//<���>
	bool ifStatement();					//<�������>
	std::string condition();			//<����>
	std::string expression();			//<���ʽ>
	std::string term();					//<��>
	std::string factor();				//<����>
	std::string returnFunctionCall();	//<�з���ֵ�����������>
	bool voidFunctionCall();			//<�޷���ֵ�����������>
	bool parameterList(int count);		//<ֵ������>
	bool forStatement();				//<forѭ�����>
	bool doWhileStatement();			//<do whileѭ�����>
	bool assignStatement();				//<��ֵ���>
	bool printfStatement();				//<д���>
	bool scanfStatement();				//<�����>
	bool returnStatement();				//<�������>
	Table getTable();
	std::vector<Quadruple> getIntermediateCodes();

	bool infoSwitch;

	bool printErrors();

private:
	std::vector <Symbol> symbles;
	IMCodeGenerator imCodeGenerator;
	TempNameManager tmpNameManager;
	LabelManager labelManager;
	int iter;
	int typeIndicator;
	std::vector<Error> syntaxErrors;
	Table table;
	std::string funcName;
	bool isVoid;
	int err;

	void printInfo(std::string str);
	void _error(int errorType, int lineNumber);
};


#endif