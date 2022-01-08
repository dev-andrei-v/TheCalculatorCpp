#include "AppMenu.h"

AppMenu::AppMenu()
{
	{
		cout << "Welcome to The Calculator.\n";
		//load previously saved variables
		if (calc.loadUserVariables())
			cout << "Your previously saved variables are loaded!\n";
		setPrecision(15);
		help();
	}
}

void AppMenu::help()
{
	cout << "\n*** Available options: \n";
	for (int i = 0; i < (int)options.size(); i++) {
		cout << (i + 1) << ". " << options[i] << "\n";
	}
}

void AppMenu::showOptions()
{
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

void AppMenu::option(int opt)
{
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

bool AppMenu::setPrecision(int p)
{
	if (p >= 0) {
		precision = p;
		return true;
	}
	else return false;
}

void AppMenu::show()
{
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
