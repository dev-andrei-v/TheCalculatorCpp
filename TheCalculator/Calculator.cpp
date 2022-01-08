#include "Calculator.h"

Calculator::Calculator()
{
}

Calculator::Calculator(string s)
{
	setInput(s);
}

void Calculator::setInput(string s)
{
	expression.setInput(s);
	evaluate();
}

double Calculator::calculate(double a, double b, char op)
{
	switch (op) {
	case '+':
		return a + b;
	case '-':
		return a - b;
	case '*':
		if (a == 0 || b == 0) return 0;
		return a * b;
	case '/':
		if (b == 0) throw exception("Can't divide by 0.");
		return a / b;
	case '^':
		return pow(a, b);
	case '#':
		return pow(a, 1 / b);
	default:
		throw exception("Invalid operator.");
	}
}

void Calculator::evaluate()
{
	stack<double> numbers;
	string output = expression.getOutput();
	int size = output.size();
	string aux = "";
	for (int i = 0; i < size; i++) {
		if (output[i] != ' ') {
			aux += output[i];
		}
		else {
			char op = aux[0];
			if (MathInput::isNumber(aux)) {
				double temp = stod(aux); //convert string to double
				numbers.push(temp);
				//cout << numbers.top();
			}
			else if (MathInput::isOperator(op))
			{
				double right = numbers.top();
				numbers.pop();
				double calc = calculate(numbers.top(), right, op);
				numbers.pop();
				numbers.push(calc);
			}
			aux = ""; //clear buffer
		}
	}
	results.push_back(numbers.top());
}

double Calculator::getLastResult()
{
	if (results.size())
		return results.back();
	else return 0;
}

bool Calculator::hasResults()
{
	return results.size() ? true : false;
}

double Calculator::operator[](int index)
{
	if (index > 0 && index < (int)results.size())
		return results[index];
}

vector<double> Calculator::getResults()
{
	return results;
}

istream& operator>>(istream& in, Calculator& c)
{
	in >> c.expression;
	c.evaluate();
	return in;
}

ostream& operator<<(ostream& out, Calculator& c)
{
	out << c.getLastResult();
	return out;
}
