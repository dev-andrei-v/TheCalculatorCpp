#pragma once
#include "CalculatorInterface.h"
#include "MathParser.h"
#include <vector>

class Calculator : public CalculatorInterface {
protected:
	vector<double> results; //save all the results the calculator evaluated
	MathParser expression;
	string output;
public:
	Calculator();
	Calculator(string s);
	virtual void setInput(string s);
	virtual double calculate(double a, double b, char op);
	void evaluate();
	double getLastResult();
	bool hasResults();
	double operator[](int index);
	vector<double> getResults();
	friend istream& operator>>(istream& in, Calculator& c);
	friend ostream& operator<<(ostream& out, Calculator& c);
};