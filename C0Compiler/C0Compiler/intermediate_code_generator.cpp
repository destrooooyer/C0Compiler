#include "intermediate_code_generator.h"

using namespace std;

TempNameManager::TempNameManager()
{
	count = 0;
}

string TempNameManager::getTempName()
{
	count++;
	return "temp" + ('0' + count);
}