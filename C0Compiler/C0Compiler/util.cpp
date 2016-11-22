#include "util.h"
#include <string>
#include <sstream>

using namespace std;

bool util::isNumber(char chr)
{
	string numbers = "0123456789";
	for (int i = 0; i < numbers.length(); i++)
		if (chr == numbers[i])
			return true;
	return false;
}

bool util::isLetter(char chr)
{
	string letters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	for (int i = 0; i < letters.length(); i++)
		if (chr == letters[i])
			return true;
	return false;
}

bool util::isChr(char chr)
{
	if (chr == 32 | chr == 33 | chr >= 35 && chr <= 126)
		return true;
	return false;
}

int util::parseInt(string str)
{
	int temp = 0;
	for (int i = 0; i < str.length(); i++)
	{
		temp *= 10;
		temp += str[i] - '0';
	}
	return temp;
}

char util::parseChar(string str)
{
	//str is like "\'a\'"
	return str[1];
}

string util::int2string(int in)
{
	stringstream ss;
	ss << in;
	string temp;
	ss >> temp;
	return temp;
}