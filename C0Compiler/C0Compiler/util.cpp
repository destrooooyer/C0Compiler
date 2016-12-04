#define _CRT_SECURE_NO_WARNINGS

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
bool util::isnumber(char chr)
{
	string numbers = "0123456789";
	for (int i = 0; i < numbers.length(); i++)
		if (chr == numbers[i])
			return true;
	if (chr == '-')
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
	return stoi(str);
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
	//stringstream ss;
	//ss << in;
	//string temp;
	//ss >> temp;
	//return ss.str();

	//string temp;
	char temp[100];
	sprintf(temp, "%d", in);
	return string(temp);

}

bool util::isArr(string str)
{
	if (isnumber(str[0]))
		return false;
	if (str[str.length() - 1] == ']')
		return true;
	else
		return false;
}

string util::getArrName(string str)
{
	string temp;
	for (int i = 0; str[i] != '['; i++)
		temp += str[i];
	return temp;
}

string util::getArrIndex(string str)
{
	int loc = 0;
	string temp;
	while (str[loc++] != '[');
	while (loc < str.length() - 1)
		temp += str[loc++];
	return temp;
}

