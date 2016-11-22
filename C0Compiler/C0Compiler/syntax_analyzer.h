#ifndef SYNTAX_ANALYZER
#define SYNTAX_ANALYZER

#include "lexical_analyzer.h"
#include "util.h"
#include "table.h"

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
	SyntaxAnalyzer(std::vector<Symble> symbles,Table table);
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
	bool condition();					//<����>
	bool expression();					//<���ʽ>
	bool term();						//<��>
	bool factor();						//<����>
	bool returnFunctionCall();			//<�з���ֵ�����������>
	bool voidFunctionCall();			//<�޷���ֵ�����������>
	bool parameterList();				//<ֵ������>
	bool forStatement();				//<forѭ�����>
	bool doWhileStatement();			//<do whileѭ�����>
	bool assignStatement();				//<��ֵ���>
	bool printfStatement();				//<д���>
	bool scanfStatement();				//<�����>
	bool returnStatement();				//<�������>
	Table getTable();

	bool infoSwitch;

	void printErrors();

private:
	std::vector <Symble> symbles;
	int iter;
	std::vector<Error> syntaxErrors;
	void printInfo(std::string str);
	void _error(int errorType, int lineNumber);
	Table table;
	std::string funcName;

};


#endif