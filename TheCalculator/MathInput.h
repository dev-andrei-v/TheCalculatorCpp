#pragma once
#include <iostream>
#include <string>
#include <stack>
using namespace std;

class MathInput {
private:
	string input;
	bool isValid = false;
public:
	MathInput();
	MathInput(string input);
	void setInput(string s);
	bool sanitaze(string& s);
	bool validate(const string& s);
	void fixString(string& s);
	void fixNegative(string& s);
	static bool isOperator(char c);
	static bool isBracket(char c);
	static bool isLeftBracket(char c);
	static bool isRightBracket(char c);
	static bool isDigit(char c);
	static bool isNumber(string s);
	static bool isDot(char c);
	static bool isSpace(char c);
	char operator[](int index);
	bool getIsValid();
	string getInput();
	int size();

	friend istream& operator>>(istream& in, MathInput& m);
	friend ostream& operator<<(ostream& out, MathInput& m);
	
};