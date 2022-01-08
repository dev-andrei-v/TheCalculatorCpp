#pragma once
#include <iostream>
#include <string>
using namespace std;

class CalculatorInterface {
public:
	virtual void setInput(string s) = 0;
	virtual void evaluate() = 0;
	virtual double calculate(double a, double b, char op) = 0;
};