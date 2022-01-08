#pragma once
#include <map>
#include <algorithm>
#include <fstream>
#include <iomanip>  
#include "Calculator.h"

class AdvancedCalculator : public Calculator {
private:
	vector<double> lastFileResults; //save all the results from the previous file read
	map<char, double> userVariables;
public:
	//clean number from multiple 0 digits
	string cleanNumber(string s);
	void setInput(string s) override;
	double calculate(double a, double b, char op) override;
	void readTxtFile(string filename);
	bool saveTxtFile(string filename, int precision);
	void saveUserVariables();
	bool loadUserVariables();
	bool insertUserVariable(char variable, double number, bool isReadFromFile = false);
	bool deleteUserVariable(char variable);
	bool deleteAllUserVariables();
	bool emptyUserVariables();
	void clearLastFileResults();
	map<char, double> getUserVariables();
	bool isReadFromFile();
	vector<double> getLastFileResults();
};
