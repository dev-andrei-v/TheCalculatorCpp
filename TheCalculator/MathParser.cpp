#include "MathParser.h"

void MathParser::removeBrackets()
{
	while (!operators.empty() && !MathInput::isLeftBracket(operators.top())) {
		string aux;
		aux = operators.top();
		aux += ' ';
		output += aux;
		operators.pop();
	}
	operators.pop();
}

void MathParser::verifyPriority(char c)
{
	//current operator priority greater than top of stack
	if (priority(c) > priority(operators.top())) {
		operators.push(c);
		return;
	}
	//current operator priority equals top of stack
	if (priority(c) == priority(operators.top())) {
		string aux;
		aux = operators.top();
		aux += ' ';
		output += aux;
		operators.pop();
		operators.push(c);
		return;
	}
	//top of stack operator priority greater than current operator priority
	if (priority(operators.top()) > priority(c)) {
		// 'ok' is to check if after these operations the stack is empty or not
		bool ok = false;
		while (!operators.empty()) {
			//we stop at brackets if there is one
			if (!MathInput::isLeftBracket(operators.top())) {
				//check for pirority
				if (priority(c) <= priority(operators.top())) {
					string aux;
					aux = operators.top();
					aux += ' ';
					output += aux;
					operators.pop();
				}
				//if 'c' priority becomes greater then stop and add 'c'
				else {
					ok = true;
					operators.push(c);
					break;
				}
			}
			//if we found brackets stop and add 'c'
			else {
				ok = true;
				operators.push(c);
				break;
			}
		}
		//if stack becomes empty stop and add 'c'
		if (!ok) {
			operators.push(c);
		}
		return;
	}
}

void MathParser::addRemainingOperators()
{
	if (!operators.empty()) {
		while (!operators.empty()) {
			string aux;
			aux = operators.top();
			aux += ' ';
			output += aux;
			operators.pop();
		}
	}
}

string MathParser::createNumber(int& pos)
{
	int size = input.size();
	string aux;
	for (; pos < size; pos++) {
		if (!MathInput::isOperator(input[pos]) && !MathInput::isRightBracket(input[pos])) {
			aux += input[pos];
		}
		else { break; }
	}
	pos--; //because I found an operator and I also need to evaluate later
	return aux;
}

MathParser::MathParser()
{
}

MathParser::MathParser(MathInput m)
{
	setInput(m.getInput());
	convert();
}

void MathParser::setInput(string s)
{
	input.setInput(s);
	convert();
}

void MathParser::convert()
{
	int size = input.size();

	for (int i = 0; i < size; i++)
	{
		//solve bracket issue
		if (MathInput::isLeftBracket(input[i])) {
			operators.push(input[i]);
		}
		else {
			if (MathInput::isRightBracket(input[i])) {
				removeBrackets();
			}
		}
		//solve operator priority issue
		if (MathInput::isOperator(input[i]) && !operators.empty()) {
			verifyPriority(input[i]);
		}
		else {
			if (MathInput::isOperator(input[i])) {
				operators.push(input[i]);
			}
		}
		//create number
		if (!MathInput::isOperator(input[i]) && !MathInput::isBracket(input[i])) {
			output += createNumber(i);
			output += ' ';
		}
	}
	//add remaining operators at the end
	addRemainingOperators();
}

int MathParser::priority(char c)
{
	switch (c) {
	case '+':
		return 1;
	case '-':
		return 1;
	case '*':
		return 2;
	case '/':
		return 2;
	case ':':
		return 2;
	case '^':
		return 3;
	case '#':
		return 3;
	default:
		return 100;
	}
}

MathInput MathParser::getInput()
{
	return input;
}

string MathParser::getOutput()
{
	return output;
}

istream& operator>>(istream& in, MathParser& m)
{
	string buffer;
	getline(in, buffer);
	m.setInput(buffer);
	return in;
}

ostream& operator<<(ostream& out, MathParser& m)
{
	cout << m.output;
	return out;
}
