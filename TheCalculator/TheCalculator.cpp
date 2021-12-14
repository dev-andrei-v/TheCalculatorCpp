
#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>  
#include <algorithm>
#include <stack>
#include <vector>
#include <map>


using namespace std;

class MathInput {
private:
	string input;
	const int inputNo; //object index
	static int inputCounter; //counts all created inputs
	bool isValid = false;
public:
	MathInput() : inputNo(0) {
		inputCounter++;
	}
	MathInput(string input) : inputNo(MathInput::inputCounter + 1) {
		setInput(input);
		inputCounter++;
	}
	MathInput(string input, int inputNo) : inputNo(MathInput::inputCounter + 1)
	{
		setInput(input);
		inputCounter++;
	}
	MathInput(const MathInput& m) : inputNo(-1) {
		input = m.input;
	}
	MathInput& operator=(const MathInput& m) {
		input = m.input;
		return *this;
	}
	char operator[](int index) {
		int size = input.size();
		if (index >= 0 && index < size)
			return input[index];
		else throw exception("Invalid index.");
	}
	//make sure there is no terminator in string to avoid further problems
	void fixString(string& s) {
		string aux;
		if (strchr(s.c_str(), '\0')) {
			for (int i = 0; i < (int)s.size(); i++)
			{
				if (s[i] != '\0')
					aux += s[i];
			}
		}
	}

	//remove spaces and all other non math-char
	bool sanitaze(string& s) {
		string aux;
		fixString(s);
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
	bool validate(const string& s) {
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
	//eg: -5 converts to 0-5
	static void fixNegative(string& s) {
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

	static bool isOperator(char c) {
		return strchr("+-*/^#:", c) ? true : false;
	}
	static bool isBracket(char c) {
		return strchr("()[]", c) ? true : false;
	}
	static bool isLeftBracket(char c) {
		return strchr("([", c) ? true : false;
	}
	static bool isRightBracket(char c) {
		return strchr(")]", c) ? true : false;
	}
	static bool isDigit(char c) {
		return strchr("0123456789", c) ? true : false;
	}
	static bool isNumber(string s) {
		int size = s.size();
		bool ok = false;
		for (int i = 0; i < size; i++)
		{
			if (isDigit(s[i]) || isDot(s[i]))
				ok = true;
		}
		return ok;
	}
	static bool isDot(char c) {
		return c == '.' ? true : false;
	}
	static bool isSpace(char c) {
		return c == ' ' ? true : false;
	}
	void setInput(string _s) {
		string s;
		//make sure there is no terminator
		for (int i = 0; _s[i] != '\0'; i++)
		{
			s += _s[i];
		}
		if (sanitaze(s) && validate(s)) {
			this->input = s;
			fixNegative(this->input);
			isValid = true;
		}
		else throw exception("Invalid input.");

	}
	bool getIsValid() {
		return isValid;
	}
	string getInput() {
		return input;
	}
	int size() {
		return input.size();
	}
	static int getInputCounter() {
		return inputCounter;
	}
	friend istream& operator>>(istream& in, MathInput& m) {
		string buffer;
		getline(in, buffer);
		m.setInput(buffer);
		return in;
	}
	friend ostream& operator<<(ostream& out, MathInput& m) {
		out << m.input << endl;
		return out;
	}
};
int MathInput::inputCounter = 0;
class MathParser {
private:
	MathInput input;
	string output;
	stack<char> operators;

	//methods used in convert
	void removeBrackets() {
		while (!operators.empty() && !MathInput::isLeftBracket(operators.top())) {
			string aux;
			aux = operators.top();
			aux += ' ';
			output += aux;
			operators.pop();
		}
		operators.pop();
	}
	void verifyPriority(char c) {
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
	void addRemainingOperators() {
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
	string createNumber(int& pos) {
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
public:
	MathParser() {

	}
	MathParser(MathInput m) {
		setInput(m.getInput());
		convert();
	}
	void setInput(string s) {
		input.setInput(s);
		convert();
	}
	//convert from INFIX to POSTFIX
	void convert() {
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

	static int priority(char c) {
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
			return -1;
		}
	}
	MathInput getInput() {
		return input;
	}
	string getOutput() {
		return output;
	}

	//read infix
	friend istream& operator>>(istream& in, MathParser& m) {
		string buffer;
		getline(in, buffer);
		m.setInput(buffer);
		return in;
	}
	//display postfix
	friend ostream& operator<<(ostream& out, MathParser& m) {
		cout << m.output;
		return out;
	}
};
class CalculatorInterface {
public:
	virtual void setInput(string s) = 0;
	virtual void evaluate() = 0;
	virtual double calculate(double a, double b, char op) = 0;

};
class Calculator : public CalculatorInterface {
protected:
	vector<double> results; //save all the results the calculator evaluated
	MathParser expression;
	string output;
public:
	Calculator() {

	}
	Calculator(string s) {
		setInput(s);
	}
	virtual void setInput(string s) {
		expression.setInput(s);
		evaluate();
	}
	virtual double calculate(double a, double b, char op) {
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
	void evaluate() {
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
	double getLastResult() {
		if (results.size())
			return results.back();
		else return 0;
	}

	bool hasResults() {
		return results.size() ? true : false;
	}
	double operator[](int index) {
		if (index > 0 && index < (int)results.size())
			return results[index];
	}

	vector<double> getResults() {
		return results;
	}

	friend istream& operator>>(istream& in, Calculator& c) {
		in >> c.expression;
		c.evaluate();
		return in;
	}
	friend ostream& operator<<(ostream& out, Calculator& c) {
		out << c.getLastResult();
		return out;
	}
};
class AdvancedCalculator : public Calculator {
private:
	vector<double> lastFileResults; //save all the results from the previous file read
	map<char, double> userVariables;
public:
	//clean number from multiple 0 digits
	string cleanNumber(string s) {
		s.erase(s.find_last_not_of('0') + 1, string::npos);
		return s;
	}
	void setInput(string s) override {
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
	double calculate(double a, double b, char op) override {
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
	void readTxtFile(string filename) {
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
	bool saveTxtFile(string filename, int precision) {
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
	void saveUserVariables() {
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
	bool loadUserVariables() {
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
	bool insertUserVariable(char variable, double number, bool isReadFromFile = false) {
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
	bool deleteUserVariable(char variable) {
		if (userVariables.count(variable) == 1) {
			//i found a variable and i can delete it
			userVariables.erase(variable);
			saveUserVariables(); //overwrite disk
			return true;
		}
		return false;


		//else return false;
	}
	bool deleteAllUserVariables() {
		userVariables.clear();
		saveUserVariables();
		return true;
	}
	bool emptyUserVariables() {
		return userVariables.empty() ? true : false;
	}

	void clearLastFileResults() {
		lastFileResults.clear();
	}

	map<char, double> getUserVariables() {
		return userVariables;
	}
	bool isReadFromFile() {
		return lastFileResults.size() ? true : false;
	}
	vector<double> getLastFileResults() {
		return lastFileResults;
	}
};
class AppMenu {
private:
	AdvancedCalculator calc;
	const vector<string> options{ "Read equation from keyboard",			    //1
						   "Read equation(s) from text file",					//2
						   "Choose where to display results",					//3
						   "Save last result in a variable (or type save)",		//4
						   "Show or delete saved variable(s)",					//5
						   "Delete all saved variable(s)",						//6
						   "Change output precision",							//7
						   "Help (or type help)",								//8
						   "Exit (or type exit)" };								//9
	enum class optionType {
		readEq = 1,
		readEqFile = 2,
		ch2display = 3,
		saveVar = 4,
		showDelVar = 5,
		delAllVar = 6,
		chPrecision = 7,
		help = 8,
		exit = 9
	};
	int precision;
	bool saveInFile = false;
public:
	AppMenu() {
		cout << "Welcome to The Calculator.\n";
		//load previously saved variables
		if (calc.loadUserVariables())
			cout << "Your previously saved variables are loaded!\n";
		setPrecision(15);
		help();
	}
	void help() {
		cout << "\n*** Available options: \n";
		for (int i = 0; i < (int)options.size(); i++) {
			cout << (i + 1) << ". " << options[i] << "\n";
		}
	}

	void showOptions() {
		int optionsNo = (int)options.size();
		int firstOption = optionsNo - (optionsNo - 1);
		cout << endl;
		cout << "** Choose an option (" << firstOption << "-" << optionsNo << "): ";
		string opt;
		cin >> opt;
		if (opt == "exit") {
			option((int)optionType::exit);
			return;
		}
		if (opt == "help") {
			option((int)optionType::help);
			return;
		}
		if (opt == "save") {
			option((int)optionType::saveVar);
			return;
		}
		if (MathInput::isNumber(opt) && stoi(opt) >= firstOption && stoi(opt) <= optionsNo) {
			option(stoi(opt));
		}
		else {
			string errmsg = "Option must be between ";
			errmsg += to_string(firstOption);
			errmsg += " and ";
			errmsg += to_string(optionsNo);
			throw exception(errmsg.c_str());
		}
	}

	void option(int opt) {
		(void)getchar(); //enter in buffer
		switch (opt) {
		case (int)optionType::readEq: {
			while (true) {
				calc.clearLastFileResults();
				cout << "To go back, type back or `" << endl;
				cout << "* Enter equation: ";
				string buffer;
				getline(cin, buffer);
				if (buffer == "back" || buffer == "BACK" || buffer == "`") {
					showOptions();
					break;
				}
				calc.setInput(buffer);
				show();
			}
			break;
		}
		case (int)optionType::readEqFile: {
			while (true) {
				cout << "To go back, type back or `" << endl;
				cout << "* Enter filename: ";
				string buffer;
				getline(cin, buffer);
				if (buffer == "back" || buffer == "BACK" || buffer == "`") {
					showOptions();
					break;
				}
				calc.readTxtFile(buffer);
				show();
			}

			break;
		}
		case (int)optionType::ch2display: {
			cout << "** Where do you want to show your results?" << endl;
			cout << "A. In the console." << endl;
			cout << "B. In a text file." << endl;
			cout << "Pick an option (A or B): ";
			char o;
			o = getchar();
			if (o == 'a' || o == 'A') {
				saveInFile = false;
			}
			if (o == 'b' || o == 'B') {
				saveInFile = true;
			}
			cout << "Your option was saved." << endl;
			showOptions();
			break;
		}
		case (int)optionType::saveVar: {
			if (calc.hasResults()) {
				cout << "Your last result was " << calc.getLastResult() << endl;
				cout << "You can save it in a variable to use it later when you insert new equation, for example a." << endl;
				cout << "Variables are case sensitive." << endl;
				cout << "Variable name: ";
				char var;
				var = getchar();
				if (!isalpha(var)) {
					cout << "You can use only letters as variables.";
					return;
				}
				if (calc.insertUserVariable(var, calc.getLastResult())) {
					cout << "You can use the variable '" << var << "' in a new equation." << endl;
					calc.saveUserVariables();
				}
				else {
					cout << "There is already a variable called " << var << ", do you want to replace it? (Y/N): ";
					char opt;
					cin >> opt;
					if (opt == 'y' || opt == 'Y') {
						calc.deleteUserVariable(var);
						calc.insertUserVariable(var, calc.getLastResult());
					}
					else cout << "I didn't change the variable content. " << endl;
				}
			}
			else {
				cout << "You can't save a variable if you didn't calculate anything before." << endl;
			}
			showOptions();
			break;
		}
		case (int)optionType::showDelVar: {
			if (calc.emptyUserVariables()) {
				cout << "You didn't save any variables." << endl;
			}
			else {
				cout << "Saved variables, which you can use later in equations: " << endl;
				map<char, double>::iterator i;
				map<char, double> var = calc.getUserVariables();
				for (i = var.begin(); i != var.end(); ++i) {
					cout << i->first << "=" << i->second << endl;
				}
				cout << "Do you want to delete a variable? (Y/N): ";
				char opt;
				cin >> opt;
				if (opt == 'Y' || opt == 'y') {
					cout << "Variable: ";
					char var;
					cin >> var;
					if (isalpha(var)) {
						if (calc.deleteUserVariable(var)) {
							cout << "'" << var << "' deleted." << endl;

						}
						else {
							cout << "'" << var << "' doesn't exist." << endl;
						}
					}
					else {
						cout << "'" << var << "' is not a valid variable." << endl;
					}
				}
			}
			break;
			break;
		}
		case (int)optionType::delAllVar: {
			cout << "Do you want to delete all saved variables? (Y/N): ";
			char opt;
			cin >> opt;
			if (opt == 'y' || opt == 'Y') {
				calc.deleteAllUserVariables();
				cout << "I deleted all the variables.\n";
			}
			else {
				cout << "I didn't touch anything. Your variables are still saved.\n";
			}
			break;
		}
		case (int)optionType::chPrecision: {
			cout << "Change calculator precision: ";
			int p;
			cin >> p;
			if (setPrecision(p))
			{
				cout << fixed << showpoint;
				cout << "Precision for new equations changed to " << p << endl;
			}
			else cout << "I couldn't change precision." << endl;
			break;
		}
		case (int)optionType::help: {
			help();
			showOptions();
			break;
		}
		case (int)optionType::exit:
			exit(0);
			break;
		}
	}

	bool setPrecision(int p) {
		if (p > 0) {
			precision = p;
			return true;
		}
		else return false;
	}

	void show() {
		if (saveInFile) {
			cout << "Output filename: ";
			string filename;
			getline(cin, filename);
			if (calc.saveTxtFile(filename, precision)) {
				cout << "* Output saved in " + filename << endl;
			}
		}
		else {
			if (calc.isReadFromFile())
			{
				vector<double> fresults = calc.getLastFileResults();
				for (int i = 0; i < (int)fresults.size(); i++)
					cout << fresults[i] << endl;
			}
			else {
				cout << setprecision(precision);
				cout << " \t\t = " << calc << endl;
			}
		}
	}

};

int main(int argc, char* argv[])
{
	bool isCmd = false;
	if (argc == 2) {
		//console eq run
		//argv[0] => program name/path
		//argv[1] => input
		string cmd(argv[1]);
		try {
			Calculator c(cmd);
			cout << c << endl;
			isCmd = true;
		}
		catch (exception e) {
			isCmd = true;
			string msg = e.what();
			cout << "Error: " << msg << endl << endl;
		}
	}
	else if (argc >= 2)
	{
		cout << "Usage: " << argv[0] << " <math input>";
		isCmd = true;
	}

	if (!isCmd) {
		AppMenu menu;
		while (true) {
			try {
				menu.showOptions();
			}
			catch (exception e) {
				cout << "Error: " << e.what() << endl;
			}
		}
	}
}