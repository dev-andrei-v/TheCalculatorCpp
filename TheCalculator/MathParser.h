#pragma once
#include <stack>
#include "MathInput.h"

class MathParser {
private:
	MathInput input;
	string output;
	stack<char> operators;

	//used in convert
	void removeBrackets();
	void verifyPriority(char c);
	void addRemainingOperators();
	string createNumber(int& pos);
public:
	MathParser();
	MathParser(MathInput m);
	void setInput(string s);
	//convert infix to postfix
	void convert();
	static int priority(char c);
	MathInput getInput();
	string getOutput();
	//read infix
	friend istream& operator>>(istream& in, MathParser& m);
	//display postfix
	friend ostream& operator<<(ostream& out, MathParser& m);
};