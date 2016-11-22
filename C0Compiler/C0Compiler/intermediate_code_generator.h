#ifndef INTERMEDIATE_CODE_GENERATOR_H
#define INTERMEDIATE_CODE_GENERATOR_H

#include "table.h"
#include <string>

class IMCodeGenerator
{

};

class TempNameManager
{
public:
	TempNameManager();
	std::string getTempName();
private:
	int count;
};

#endif
