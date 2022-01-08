#include "AdvancedCalculator.h"

string AdvancedCalculator::cleanNumber(string s)
{
	s.erase(s.find_last_not_of('0') + 1, string::npos);
	return s;
}

void AdvancedCalculator::setInput(string s)
{
	if (!userVariables.empty()) {
		//replace chars with actual user variables
		string str;
		for (int i = 0; i < (int)s.size(); i++) {
			for (auto itr = userVariables.find(s[i]); itr != userVariables.end(); itr++) {
				if (s[i] == itr->first)
				{
					str += cleanNumber(to_string(itr->second));
					i++; //jump to next
				}
			}
			str += s[i];

		}
		expression.setInput(str);
	}
	else {
		expression.setInput(s);
	}
	evaluate();
}

double AdvancedCalculator::calculate(double a, double b, char op)
{
	switch (op) {
	case '+':
		return a + b;
	case '-':
		return a - b;
	case '*':
		if (a == 0 || b == 0) return 0;
		return a * b;
	case '×':
		if (a == 0 || b == 0) return 0;
		return a * b;
	case 'x':
		if (a == 0 || b == 0) return 0;
		return a * b;
	case '/':
		if (b == 0) throw exception("Can't divide by 0.");
		return a / b;
	case ':':
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

void AdvancedCalculator::readTxtFile(string filename)
{
	{
		lastFileResults.clear(); //empty before reading others eq
		ifstream f(filename);
		if (f)
		{
			string line;
			while (getline(f, line)) {
				AdvancedCalculator c;
				c.setInput(line);
				lastFileResults.push_back(c.getLastResult());
				results.push_back(c.getLastResult());
			}


		}

		f.close();
	}
}

bool AdvancedCalculator::saveTxtFile(string filename, int precision)
{
	ofstream g(filename);
	if (g)
	{
		g << setprecision(precision);
		if (isReadFromFile()) {
			for (int i = 0; i < (int)lastFileResults.size(); i++) {
				g << lastFileResults[i] << endl;
			}
		}
		else {
			if (hasResults()) {
				g << getLastResult() << endl;
			}
		}
		return true;
	}
	else return false;
	g.close();
}

void AdvancedCalculator::saveUserVariables()
{
	ofstream f;
	f.open("uservariables.bin", ios::binary);
	//save into binary files only if userVariables has elements
	if (userVariables.size() > 0) {
		map<char, double>::iterator it;
		for (it = userVariables.begin(); it != userVariables.end(); it++) {
			//key
			f.write((char*)&it->first, sizeof(it->first));
			//value
			f.write((char*)&it->second, sizeof(it->second));
		}
		f.close();
	}
}

bool AdvancedCalculator::loadUserVariables()
{
	ifstream f("uservariables.bin", ios::binary);
	char buffchr;
	double buffvar;
	bool isFileEmpty = false;
	f.seekg(0, ios::end);
	if (f.tellg() == 0) isFileEmpty = true;
	f.seekg(0); //go back to file start
	if (f && !isFileEmpty) {
		while (!f.eof()) {
			f.read((char*)&buffchr, sizeof(buffchr));
			f.read((char*)&buffvar, sizeof(buffvar));
			insertUserVariable(buffchr, buffvar, true);
		}
		f.close();
		return true;
	}
	return false;
}

bool AdvancedCalculator::insertUserVariable(char variable, double number, bool isReadFromFile)
{
	map<char, double>::iterator it;
	it = userVariables.find(variable);
	//check if variable already exists
	if (it == userVariables.end()) {
		if (binary_search(results.begin(), results.end(), number)) {
			userVariables.insert(pair<char, double>(variable, number));
			return true;
		}
		else if (isReadFromFile) {
			userVariables.insert(pair<char, double>(variable, number));
			return true;
		}
	}
	return false;
}

bool AdvancedCalculator::deleteUserVariable(char variable)
{
	if (userVariables.count(variable) == 1) {
		//i found a variable and i can delete it
		userVariables.erase(variable);
		saveUserVariables(); //overwrite disk
		return true;
	}
	return false;
}

bool AdvancedCalculator::deleteAllUserVariables()
{
	userVariables.clear();
	saveUserVariables();
	return true;
}

bool AdvancedCalculator::emptyUserVariables()
{
	return userVariables.empty() ? true : false;
}

void AdvancedCalculator::clearLastFileResults()
{
	lastFileResults.clear();
}

map<char, double> AdvancedCalculator::getUserVariables()
{
	return userVariables;
}

bool AdvancedCalculator::isReadFromFile()
{
	return lastFileResults.size() ? true : false;
}

vector<double> AdvancedCalculator::getLastFileResults()
{
	return lastFileResults;
}
