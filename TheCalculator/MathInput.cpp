#include "MathInput.h"

MathInput::MathInput()
{
}

MathInput::MathInput(string input)
{
	setInput(input);
}

void MathInput::setInput(string s)
{
	fixString(s);
	if (sanitaze(s) && validate(s)) {
		this->input = s;
		fixNegative(this->input);
		isValid = true;
	}
	else throw exception("Invalid input.");
}

bool MathInput::sanitaze(string& s)
{
	string aux;
	int size = s.size();
	for (int i = 0; i < size; i++) {
		if (isDigit(s[i]) || isDot(s[i]) || isOperator(s[i]) || isBracket(s[i]))
			aux += s[i];
		else if (!isSpace(s[i]))
			return false;
	}
	s = aux;
	return true;
}

bool MathInput::validate(const string& s)
{
	stack<char> brackets1; //[ ]
	stack<char> brackets2; //( )
	int size = s.size();
	//allow operator '-' only at the beginning
	//don't allow single dot input
	if ((isOperator(s[0]) && s[0] != '-') || isDot(s[0])) {
		return false;
	}
	bool containsAnyDot = false;
	if (strchr(s.c_str(), '.')) containsAnyDot = true;
	for (int i = 0; i < size; i++)
	{
		//brackets validation
		if (strchr("])", s[i]) && brackets1.empty() && brackets2.empty())
		{
			return false;
		}
		if (s[i] == '[') {
			brackets1.push('[');
		}
		else {
			if (s[i] == ']' && !brackets1.empty()) {
				brackets1.pop();
			}
		}

		if (s[i] == '(') {
			brackets2.push('(');
		}
		else {
			if (s[i] == ')' && !brackets2.empty()) {
				brackets2.pop();
			}
		}

		//enter only if string contains a dot
		if (containsAnyDot) {
			//dot validation
			if (isDot(s[i])) {
				//s[i] is a dot
				for (int j = i + 1; j < size; j++)
				{
					//check for multiple dots back to back
					//eg.: .., 2.2.3
					if (isDot(s[j]) && s[i] == s[j])
						return false;
					else
						if (isOperator(s[j]))
							break;
				}
			}
		}
		//operators validation
		if (isOperator(s[i]))
		{
			//s[i] is an operator
			for (int j = i + 1; j < size; j++)
			{
				//check for same/different operators back to back
				//eg. of ilegall operators: ++, --, -^ 

				if (isOperator(s[j]) && (s[j] == s[i] || s[j] != s[i])) {
					//s[j] is an operator
					return false;
				}
				else {
					//divide by 0 case
					if (s[i] == '/' && s[j] == '0')
					{
						throw exception("Can't divide by 0.");
						return false;
					}
					if (!isOperator(s[j]))
					{
						break;
					}

				}

			}
		}
	}
	if (!brackets1.empty() || !brackets2.empty())
	{
		return false;
	}

	//single operator at the end of the input
	if (isOperator(s[size - 1])) {
		return false;
	}

	return true;
}

void MathInput::fixString(string& s)
{
	string aux;
	if (strchr(s.c_str(), '\0')) {
		for (int i = 0; i < (int)s.size(); i++)
		{
			if (s[i] != '\0')
				aux += s[i];
		}
	}
	s = aux;
}

void MathInput::fixNegative(string& s)
{
	int size = s.size();
	string aux;
	if (s[0] == '-')
		aux += "0-";
	else {
		aux += s[0];
	};

	for (int i = 1; i < size; i++) {
		if (s[i] != '-')
			aux += s[i];

		else {
			if (isLeftBracket(s[i - 1]) && isDigit(s[i + 1]))
			{
				aux += '0';
				aux += s[i];
			}
			else aux += s[i];
		}
	}
	s = aux;
}

bool MathInput::isOperator(char c)
{
	return strchr("+-*/^#:", c) ? true : false;
}

bool MathInput::isBracket(char c)
{
	return strchr("()[]", c) ? true : false;
}

bool MathInput::isLeftBracket(char c)
{
	return strchr("([", c) ? true : false;
}

bool MathInput::isRightBracket(char c)
{
	return strchr(")]", c) ? true : false;
	return false;
}

bool MathInput::isDigit(char c)
{
	return strchr("0123456789", c) ? true : false;
	return false;
}

bool MathInput::isNumber(string s)
{
	int size = s.size();
	bool ok = false;
	for (int i = 0; i < size; i++)
	{
		if (isDigit(s[i]) || isDot(s[i]))
			ok = true;
	}
	return ok;
}

bool MathInput::isDot(char c)
{
	return c == '.' ? true : false;
}

bool MathInput::isSpace(char c)
{
	return c == ' ' ? true : false;
}

char MathInput::operator[](int index)
{
	int size = input.size();
	if (index >= 0 && index < size)
		return input[index];
	else throw exception("Invalid index.");
}

bool MathInput::getIsValid()
{
	return isValid;
}

string MathInput::getInput()
{
	return input;
}

int MathInput::size()
{
	return input.size();
}

istream& operator>>(istream& in, MathInput& m)
{
	string buffer;
	getline(in, buffer);
	m.setInput(buffer);
	return in;
}

ostream& operator<<(ostream& out, MathInput& m)
{
	out << m.input << endl;
	return out;
}
